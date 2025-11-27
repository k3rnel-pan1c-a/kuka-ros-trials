#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.hpp>
#include <my_robot_interfaces/msg/pose_command.hpp>
#include <my_robot_interfaces/srv/get_current_pose.hpp>
#include <my_robot_interfaces/srv/do_homing.hpp>
#include <my_robot_interfaces/srv/do_calibration.hpp>
#include <std_msgs/msg/string.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>

#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>

#include <cmath>
#include <atomic>
#include <memory>

using MoveGroupInterface = moveit::planning_interface::MoveGroupInterface;

using PoseCommand = my_robot_interfaces::msg::PoseCommand;
using GetCurrentPose = my_robot_interfaces::srv::GetCurrentPose;
using DoHoming = my_robot_interfaces::srv::DoHoming;
using DoCalibration = my_robot_interfaces::srv::DoCalibration;

using String = std_msgs::msg::String;
using LaserScan = sensor_msgs::msg::LaserScan;


using namespace std::placeholders;


class Commander {
public:
    Commander(const std::shared_ptr<rclcpp::Node>& main_node,
              const std::shared_ptr<rclcpp::Node>& feedback_node,
              const std::shared_ptr<rclcpp::Node>& moveit_node,
              std::shared_ptr<std::atomic<bool>> is_valid_range_flag,
              std::shared_ptr<std::atomic<bool>> is_calibrating_flag,
              double max_radius, double end_effector_yaw, double min_z, double max_z)
        : main_node_(main_node),
          feedback_node_(feedback_node),
          moveit_node_(moveit_node),
          max_radius_(max_radius),
          min_z_(min_z),
          max_z_(max_z),
          end_effector_yaw(end_effector_yaw),
          is_valid_range_(is_valid_range_flag),
          is_calibrating_(is_calibrating_flag)

    {
        arm_group_ = std::make_shared<MoveGroupInterface>(
            moveit_node_,
            MoveGroupInterface::Options("arm_group", "robot_description", "")
        );

        arm_group_->setMaxAccelerationScalingFactor(1.0);
        arm_group_->setMaxVelocityScalingFactor(1.0);

        go_to_named_target_sub_ = main_node_->create_subscription<String>(
            "named_target", 10,
            std::bind(&Commander::namedTargetCmdCallback, this, _1));

        laser_scan_sub_ = feedback_node_->create_subscription<LaserScan>(
            "scan", 10,
            std::bind(&Commander::laserScanCallback, this, _1));

        get_current_pose_service_ = main_node_->create_service<GetCurrentPose>(
            "get_current_pose",
            std::bind(&Commander::getCurrentPoseService, this, _1, _2));

        do_homing_service_ = main_node_->create_service<DoHoming>(
            "do_homing",
            std::bind(&Commander::doHomingService, this, _1, _2));
        
        do_calibration_service_ = main_node_->create_service<DoCalibration>(
            "do_calibration",
            std::bind(&Commander::doCalibrationService, this, _1, _2));
        
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

    void namedTargetCmdCallback(const String &msg) {
        goToNamedTarget(msg.data);
    }

    void laserScanCallback(const LaserScan &msg){
        if (*is_calibrating_ && !(*is_valid_range_)) {
            for (size_t i = 0; i < msg.ranges.size(); i++){
                if(std::isfinite(msg.ranges[i])){
                    *is_valid_range_ = true; 
                    RCLCPP_INFO(main_node_->get_logger(), "Valid range detected!");
                    break;
                }
            }
        }
    }

    void getCurrentPoseService(
        const std::shared_ptr<GetCurrentPose::Request> request,
        std::shared_ptr<GetCurrentPose::Response> response)
    {
        (void)request;
        
        try {
            auto current_pose = arm_group_->getCurrentPose();
            
            response->x = current_pose.pose.position.x;
            response->y = current_pose.pose.position.y;
            response->z = current_pose.pose.position.z;
            
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
        (void)request; 
        
        try {
            RCLCPP_INFO(main_node_->get_logger(), 
                        "Homing service called - moving to home position [%.3f, %.3f, %.3f] with yaw: %.3f",
                        -max_radius_, -max_radius_, max_z_, end_effector_yaw);
            
            bool success = goToPoseTarget(-max_radius_, -max_radius_, max_z_, 0, 0, end_effector_yaw, false);
            
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
    
    void doCalibrationService(
        const std::shared_ptr<DoCalibration::Request> request,
        std::shared_ptr<DoCalibration::Response> response)
    {
        try {
            double step_size = request->step_size;
            
            if (step_size <= 0.0 || step_size > 0.1) {
                response->success = false;
                response->message = "Invalid step size. Must be between 0 and 0.1 meters.";
                RCLCPP_WARN(main_node_->get_logger(), "%s", response->message.c_str());
                return;
            }
            
            RCLCPP_INFO(main_node_->get_logger(), 
                        "Calibration service called with step size: %.3f", step_size);
            
            *is_calibrating_ = true;
            *is_valid_range_ = false;  
            
            RCLCPP_INFO(main_node_->get_logger(), "Calibration mode enabled");
            
            approachWithVerticalScans(step_size);
            
            *is_calibrating_ = false;
            RCLCPP_INFO(main_node_->get_logger(), "Calibration mode disabled");
            
            if (*is_valid_range_) {
                response->success = true;
                response->message = "Calibration completed successfully - valid range found";
                RCLCPP_INFO(main_node_->get_logger(), "%s", response->message.c_str());
            } else {
                response->success = false;
                response->message = "Calibration completed but no valid range found";
                RCLCPP_WARN(main_node_->get_logger(), "%s", response->message.c_str());
            }
            
        } catch (const std::exception& e) {
            *is_calibrating_ = false;
            response->success = false;
            response->message = std::string("Calibration failed: ") + e.what();
            RCLCPP_ERROR(main_node_->get_logger(), "%s", response->message.c_str());
        }
    }

    void doVerticalScan(){
        if (!is_valid_range_) {
            RCLCPP_ERROR(main_node_->get_logger(), "is_valid_range_ pointer is null!");
            return;
        }
        
        RCLCPP_INFO(main_node_->get_logger(), "Getting current pose...");
        auto current_pose = arm_group_->getCurrentPose();
        double x = current_pose.pose.position.x;
        double y = current_pose.pose.position.y;
        
        RCLCPP_INFO(main_node_->get_logger(), "Current position: [%.3f, %.3f]", x, y);
        
        RCLCPP_INFO(main_node_->get_logger(), 
                    "Starting vertical scan from z=%.3f to z=%.3f at position [%.3f, %.3f]", max_z_, min_z_, x, y);
        
        bool success = goToPositionTarget(x, y, min_z_, true);
        
        if (!success) {
            RCLCPP_WARN(main_node_->get_logger(), 
                        "Failed to complete vertical scan movement");
            return;
        }

        if (*is_valid_range_) {
            RCLCPP_INFO(main_node_->get_logger(), 
                        "Valid range detected during vertical scan!");
        } else {
            RCLCPP_WARN(main_node_->get_logger(), 
                        "Vertical scan completed - no valid range found from z=%.3f to z=%.3f", max_z_, min_z_);
        }
    }

    void approachWithVerticalScans(double step_size = 0.1){
        if (!is_valid_range_) {
            RCLCPP_ERROR(main_node_->get_logger(), "is_valid_range_ pointer is null!");
            return;
        }

        RCLCPP_INFO(main_node_->get_logger(), 
                    "Starting approach with vertical scans (step size: %.3f)", step_size);
        
        auto current_pose = arm_group_->getCurrentPose();
        double x = current_pose.pose.position.x;
        double y = current_pose.pose.position.y;
        
        RCLCPP_INFO(main_node_->get_logger(), 
                    "Initial position: [%.3f, %.3f]", x, y);
        
        double distance = std::sqrt(x * x + y * y);
        double step_x = -(x / distance) * step_size;
        double step_y = -(y / distance) * step_size;
        
        RCLCPP_INFO(main_node_->get_logger(), 
                    "Step direction: [%.3f, %.3f] (towards origin)", step_x, step_y);
        
        int iteration = 0;
        const int max_iterations = 50; 
        
        while (!(*is_valid_range_) && iteration < max_iterations) {
            RCLCPP_INFO(main_node_->get_logger(), 
                        "Iteration %d: Starting vertical scan at [%.3f, %.3f]", 
                        iteration, x, y);
            
            doVerticalScan();
            
            if (*is_valid_range_) {
                RCLCPP_INFO(main_node_->get_logger(), 
                            "Valid range found at position [%.3f, %.3f] after %d iterations", 
                            x, y, iteration);
                break;
            }
            
            x += step_x;
            y += step_y;
            
            RCLCPP_INFO(main_node_->get_logger(), 
                        "Moving to next scan position: [%.3f, %.3f, %.3f]", x, y, max_z_);
            bool success = goToPositionTarget(x, y, max_z_, true);
            
            if (!success) {
                RCLCPP_ERROR(main_node_->get_logger(), 
                            "Failed to move to next scan position. Aborting approach.");
                return;
            }
            
            iteration++;
        }
        
        if (iteration >= max_iterations) {
            RCLCPP_WARN(main_node_->get_logger(), 
                        "Reached maximum iterations (%d) without finding valid range", 
                        max_iterations);
        }
        else{
            RCLCPP_INFO(main_node_->get_logger(), 
                        "Approach with vertical scans completed. Final position: [%.3f, %.3f]", 
                        x, y);
        }
    }


    std::shared_ptr<rclcpp::Node> main_node_;
    std::shared_ptr<rclcpp::Node> feedback_node_;
    std::shared_ptr<rclcpp::Node> moveit_node_;

    std::shared_ptr<MoveGroupInterface> arm_group_;

    rclcpp::Subscription<String>::SharedPtr go_to_named_target_sub_;
    rclcpp::Subscription<LaserScan>::SharedPtr laser_scan_sub_;

    rclcpp::Service<GetCurrentPose>::SharedPtr get_current_pose_service_;
    rclcpp::Service<DoHoming>::SharedPtr do_homing_service_;
    rclcpp::Service<DoCalibration>::SharedPtr do_calibration_service_;
    
    const double max_radius_;
    const double min_z_;
    const double max_z_;

    const double end_effector_yaw;

    std::shared_ptr<std::atomic<bool>> is_valid_range_;
    std::shared_ptr<std::atomic<bool>> is_calibrating_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    auto is_valid_range_flag = std::make_shared<std::atomic<bool>>(false);
    auto is_calibrating_flag = std::make_shared<std::atomic<bool>>(false);

    auto main_node = std::make_shared<rclcpp::Node>("CommanderNode");
    auto feedback_node = std::make_shared<rclcpp::Node>("FeedbackNode");
    auto moveit_node = std::make_shared<rclcpp::Node>("MoveItNode");
    
    double max_radius = 0.254;
    double min_laser_range = 0.19;

    double max_height = 0.6;
    double drill_bit_joint_z = 1.4; 
    double max_z = drill_bit_joint_z + max_height / 2;
    double min_z = drill_bit_joint_z - max_height / 2;

    double end_effector_yaw = M_PI / 4;

    auto commander = std::make_shared<Commander>(main_node, feedback_node, moveit_node, is_valid_range_flag, is_calibrating_flag,
                                                   max_radius + min_laser_range, end_effector_yaw, min_z, max_z);

    rclcpp::executors::MultiThreadedExecutor executor(
        rclcpp::ExecutorOptions(),
        4 
    );
    executor.add_node(main_node);
    executor.add_node(feedback_node);
    executor.add_node(moveit_node);
    executor.spin();

    rclcpp::shutdown();
    return 0;
}
