#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.hpp>
#include <my_robot_interfaces/msg/pose_command.hpp>
#include <std_msgs/msg/string.hpp>

#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>

using MoveGroupInterface = moveit::planning_interface::MoveGroupInterface;
using PoseCommand = my_robot_interfaces::msg::PoseCommand;
using String = std_msgs::msg::String;
using namespace std::placeholders;


class Commander {
public:
    Commander(const std::shared_ptr<rclcpp::Node>& main_node)
        : main_node_(main_node)
    {
        // ----------------------------------------------------
        // 1) Create SEPARATE MoveIt node
        // ----------------------------------------------------
        moveit_node_ = std::make_shared<rclcpp::Node>("moveit_node");

        // ----------------------------------------------------
        // 2) Create custom executor for MoveIt
        // ----------------------------------------------------
        executor_ = std::make_shared<rclcpp::executors::SingleThreadedExecutor>();
        executor_->add_node(moveit_node_);

        executor_thread_ = std::thread([this]() {
            RCLCPP_INFO(main_node_->get_logger(), "MoveIt executor thread started");
            executor_->spin();
        });

        // ----------------------------------------------------
        // 3) Initialize MoveGroupInterface using moveit_node_
        // ----------------------------------------------------
        arm_group = std::make_shared<MoveGroupInterface>(
            moveit_node_,
            MoveGroupInterface::Options("arm_group", "robot_description", "")
        );

        arm_group->setMaxAccelerationScalingFactor(1.0);
        arm_group->setMaxVelocityScalingFactor(1.0);

        // ----------------------------------------------------
        // 4) Subscribers MUST be created on the main node
        // ----------------------------------------------------
        pose_cmd_sub_ = main_node_->create_subscription<PoseCommand>(
            "pose_command", 10,
            std::bind(&Commander::poseCmdCallback, this, _1));

        position_cmd_sub_ = main_node_->create_subscription<PoseCommand>(
            "position_command", 10,
            std::bind(&Commander::positionCmdCallback, this, _1));

        go_to_named_target_sub_ = main_node_->create_subscription<String>(
            "named_target", 10,
            std::bind(&Commander::namedTargetCmdCallback, this, _1));
    }

    ~Commander() {
        executor_->cancel();
        if (executor_thread_.joinable())
            executor_thread_.join();
    }

    // --------------------------------------------------------------------
    // Public movement commands
    // --------------------------------------------------------------------

    void goToNamedTarget(const std::string &name) {
        arm_group->setStartStateToCurrentState();
        arm_group->setNamedTarget(name);
        planAndExecute(arm_group);
    }

    void goToPoseTarget(double x, double y, double z,
                        double roll, double pitch, double yaw,
                        bool cartesian_path = false)
    {
        RCLCPP_INFO(main_node_->get_logger(),
                    "End Effector Link: %s",
                    arm_group->getEndEffectorLink().c_str());

        tf2::Quaternion q;
        q.setRPY(roll, pitch, yaw);
        q.normalize();

        geometry_msgs::msg::PoseStamped target_pose;
        target_pose.header.frame_id = "base_link";
        target_pose.pose.position.x = x;
        target_pose.pose.position.y = y;
        target_pose.pose.position.z = z;
        target_pose.pose.orientation.x = q.getX();
        target_pose.pose.orientation.y = q.getY();
        target_pose.pose.orientation.z = q.getZ();
        target_pose.pose.orientation.w = q.getW();

        arm_group->setStartStateToCurrentState();

        if (!cartesian_path) {
            arm_group->setPoseTarget(target_pose);
            planAndExecute(arm_group);
        } else {
            std::vector<geometry_msgs::msg::Pose> waypoints;
            waypoints.push_back(target_pose.pose);

            moveit_msgs::msg::RobotTrajectory trajectory;
            double fraction = arm_group->computeCartesianPath(
                waypoints, 0.01, trajectory);

            if (fraction == 1.0) {
                arm_group->execute(trajectory);
            }
        }
    }

    void goToPositionTarget(double x, double y, double z,
                            bool cartesian_path = false)
    {
        auto current_pose = arm_group->getCurrentPose();

        tf2::Quaternion q(
            current_pose.pose.orientation.x,
            current_pose.pose.orientation.y,
            current_pose.pose.orientation.z,
            current_pose.pose.orientation.w
        );

        double roll, pitch, yaw;
        tf2::Matrix3x3(q).getRPY(roll, pitch, yaw);

        goToPoseTarget(x, y, z, roll, pitch, yaw, cartesian_path);
    }

private:
    // --------------------------------------------------------------------
    // Planner helper
    // --------------------------------------------------------------------

    void planAndExecute(const std::shared_ptr<MoveGroupInterface>& interface) {
        MoveGroupInterface::Plan plan;
        bool success = (interface->plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);

        if (success)
            interface->execute(plan);
    }

    // --------------------------------------------------------------------
    // Callbacks
    // --------------------------------------------------------------------

    void poseCmdCallback(const PoseCommand &msg) {
        goToPoseTarget(msg.x, msg.y, msg.z,
                       msg.roll, msg.pitch, msg.yaw,
                       msg.cartesian_path);
    }

    void positionCmdCallback(const PoseCommand &msg) {
        goToPositionTarget(msg.x, msg.y, msg.z,
                           msg.cartesian_path);
    }

    void namedTargetCmdCallback(const String &msg) {
        goToNamedTarget(msg.data);
    }

    // --------------------------------------------------------------------
    // Members
    // --------------------------------------------------------------------
    std::shared_ptr<rclcpp::Node> main_node_;       // User-facing node
    std::shared_ptr<rclcpp::Node> moveit_node_;     // MoveGroupInterface node

    std::shared_ptr<MoveGroupInterface> arm_group;

    rclcpp::Subscription<PoseCommand>::SharedPtr pose_cmd_sub_;
    rclcpp::Subscription<PoseCommand>::SharedPtr position_cmd_sub_;
    rclcpp::Subscription<String>::SharedPtr go_to_named_target_sub_;

    std::shared_ptr<rclcpp::Executor> executor_;
    std::thread executor_thread_;
};


// ========================================================================
// MAIN
// ========================================================================

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    auto main_node = std::make_shared<rclcpp::Node>("CommanderNode");
    auto commander = std::make_shared<Commander>(main_node);

    rclcpp::spin(main_node);

    rclcpp::shutdown();
    return 0;
}
