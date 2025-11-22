#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.hpp>
#include <my_robot_interfaces/msg/pose_command.hpp>
#include <my_robot_interfaces/srv/get_current_pose.hpp>
#include <my_robot_interfaces/srv/do_homing.hpp>
#include <std_msgs/msg/string.hpp>

#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>

#include <cmath>

using MoveGroupInterface = moveit::planning_interface::MoveGroupInterface;

using PoseCommand = my_robot_interfaces::msg::PoseCommand;
using GetCurrentPose = my_robot_interfaces::srv::GetCurrentPose;
using DoHoming = my_robot_interfaces::srv::DoHoming;

using String = std_msgs::msg::String;
using namespace std::placeholders;


class Commander {
public:
    Commander(const std::shared_ptr<rclcpp::Node>& main_node, 
              double max_radius, 
              double max_height, double end_effector_yaw)
        : main_node_(main_node),
          maxRadius(max_radius),
          maxHeight(max_height),
          end_effector_yaw(end_effector_yaw)

    {
        moveit_node_ = std::make_shared<rclcpp::Node>("moveit_node");

        executor_ = std::make_shared<rclcpp::executors::SingleThreadedExecutor>();
        executor_->add_node(moveit_node_);

        executor_thread_ = std::thread([this]() {
            RCLCPP_INFO(main_node_->get_logger(), "MoveIt executor thread started");
            executor_->spin();
        });

        arm_group_ = std::make_shared<MoveGroupInterface>(
            moveit_node_,
            MoveGroupInterface::Options("arm_group", "robot_description", "")
        );

        arm_group_->setMaxAccelerationScalingFactor(1.0);
        arm_group_->setMaxVelocityScalingFactor(1.0);

        pose_cmd_sub_ = main_node_->create_subscription<PoseCommand>(
            "pose_command", 10,
            std::bind(&Commander::poseCmdCallback, this, _1));

        position_cmd_sub_ = main_node_->create_subscription<PoseCommand>(
            "position_command", 10,
            std::bind(&Commander::positionCmdCallback, this, _1));

        go_to_named_target_sub_ = main_node_->create_subscription<String>(
            "named_target", 10,
            std::bind(&Commander::namedTargetCmdCallback, this, _1));

        get_current_pose_service_ = main_node_->create_service<GetCurrentPose>(
            "get_current_pose",
            std::bind(&Commander::getCurrentPoseService, this, _1, _2));

        do_homing_service_ = main_node_->create_service<DoHoming>(
            "do_homing",
            std::bind(&Commander::doHomingService, this, _1, _2));
    }

    ~Commander() {
        executor_->cancel();
        if (executor_thread_.joinable())
            executor_thread_.join();
    }

    bool goToNamedTarget(const std::string &name) {
        arm_group_->setStartStateToCurrentState();
        arm_group_->setNamedTarget(name);
        return planAndExecute(arm_group_);
    }

    bool goToPoseTarget(double x, double y, double z,
                        double roll, double pitch, double yaw,
                        bool cartesian_path = false)
    {
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

        arm_group_->setStartStateToCurrentState();

        if (!cartesian_path) {
            arm_group_->setPoseTarget(target_pose);
            return planAndExecute(arm_group_);
        } else {
            std::vector<geometry_msgs::msg::Pose> waypoints;
            waypoints.push_back(target_pose.pose);

            moveit_msgs::msg::RobotTrajectory trajectory;
            double fraction = arm_group_->computeCartesianPath(
                waypoints, 0.01, trajectory);

            if (fraction == 1.0) {
                auto result = arm_group_->execute(trajectory);
                return (result == moveit::core::MoveItErrorCode::SUCCESS);
            }
            return false;
        }
    }

    bool goToPositionTarget(double x, double y, double z,
                            bool cartesian_path = false)
    {
        auto current_pose = arm_group_->getCurrentPose();

        tf2::Quaternion q(
            current_pose.pose.orientation.x,
            current_pose.pose.orientation.y,
            current_pose.pose.orientation.z,
            current_pose.pose.orientation.w
        );

        double roll, pitch, yaw;
        tf2::Matrix3x3(q).getRPY(roll, pitch, yaw);

        return goToPoseTarget(x, y, z, roll, pitch, yaw, cartesian_path);
    }


private:
    bool planAndExecute(const std::shared_ptr<MoveGroupInterface>& interface) {
        MoveGroupInterface::Plan plan;
        bool success = (interface->plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);

        if (success)
            interface->execute(plan);
        
        return success;
    }

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

    void getCurrentPoseService(
        const std::shared_ptr<GetCurrentPose::Request> request,
        std::shared_ptr<GetCurrentPose::Response> response)
    {
        (void)request;  // Unused parameter (I guess we do this to not get any compiler errors?)
        
        try {
            auto current_pose = arm_group_->getCurrentPose();
            
            // Extract position
            response->x = current_pose.pose.position.x;
            response->y = current_pose.pose.position.y;
            response->z = current_pose.pose.position.z;
            
            // Convert quaternion to roll, pitch, yaw
            tf2::Quaternion q(
                current_pose.pose.orientation.x,
                current_pose.pose.orientation.y,
                current_pose.pose.orientation.z,
                current_pose.pose.orientation.w
            );
            
            double roll, pitch, yaw;
            tf2::Matrix3x3(q).getRPY(roll, pitch, yaw);
            
            response->roll = roll;
            response->pitch = pitch;
            response->yaw = yaw;
            
            response->success = true;
            response->message = "Successfully retrieved current pose";
            
            RCLCPP_INFO(main_node_->get_logger(),
                        "Service called - Current pose: position [%.3f, %.3f, %.3f] orientation [roll: %.3f, pitch: %.3f, yaw: %.3f]",
                        response->x, response->y, response->z,
                        response->roll, response->pitch, response->yaw);
        } catch (const std::exception& e) {
            response->success = false;
            response->message = std::string("Failed to get current pose: ") + e.what();
            RCLCPP_ERROR(main_node_->get_logger(), "%s", response->message.c_str());
        }
    }

    void doHomingService(
        const std::shared_ptr<DoHoming::Request> request,
        std::shared_ptr<DoHoming::Response> response)
    {
        (void)request; // unused parameter
        
        try {
            RCLCPP_INFO(main_node_->get_logger(), 
                        "Homing service called - moving to home position [%.3f, %.3f, %.3f] with yaw: %.3f",
                        -maxRadius, -maxRadius, maxHeight, end_effector_yaw);
            
            bool success = goToPoseTarget(-maxRadius, -maxRadius, maxHeight, 0, 0, end_effector_yaw, false);
            
            response->success = success;
            
            if (success) {
                RCLCPP_INFO(main_node_->get_logger(), "Homing completed successfully");
            } else {
                RCLCPP_WARN(main_node_->get_logger(), "Homing failed - planning or execution error");
            }
        } catch (const std::exception& e) {
            response->success = false;
            RCLCPP_ERROR(main_node_->get_logger(), "Homing failed: %s", e.what());
        }
    }

    std::shared_ptr<rclcpp::Node> main_node_;       // User-facing node
    std::shared_ptr<rclcpp::Node> moveit_node_;     // MoveGroupInterface node

    std::shared_ptr<MoveGroupInterface> arm_group_;

    rclcpp::Subscription<PoseCommand>::SharedPtr pose_cmd_sub_;
    rclcpp::Subscription<PoseCommand>::SharedPtr position_cmd_sub_;
    rclcpp::Subscription<String>::SharedPtr go_to_named_target_sub_;

    rclcpp::Service<GetCurrentPose>::SharedPtr get_current_pose_service_;
    rclcpp::Service<DoHoming>::SharedPtr do_homing_service_;

    std::shared_ptr<rclcpp::Executor> executor_;
    std::thread executor_thread_;
    
    const double maxRadius;
    const double maxHeight;
    const double end_effector_yaw;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    auto main_node = std::make_shared<rclcpp::Node>("CommanderNode");
    
    double max_radius = 0.25;  
    double max_height = 1.5; 
    double end_effector_yaw = M_PI / 4;  // 45 degrees 

    auto commander = std::make_shared<Commander>(main_node, max_radius, max_height, end_effector_yaw);

    rclcpp::spin(main_node);

    rclcpp::shutdown();
    return 0;
}
