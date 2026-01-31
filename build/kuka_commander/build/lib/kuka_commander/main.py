#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from moveit.planning import MoveItPy
from moveit.planning import PlanningComponent
from moveit_configs_utils import MoveItConfigsBuilder
from geometry_msgs.msg import PoseStamped, Pose
from std_msgs.msg import String
import tf_transformations

from example_interfaces.msg import Bool
from example_interfaces.msg import Float64MultiArray
from my_robot_interfaces.msg import PoseCommand
from geometry_msgs.msg import Point


ROBOT_CONFIG = MoveItConfigsBuilder(robot_name="kuka_kr10r1420", package_name="kuka_config")\
                                    .robot_description_semantic("config/kuka_kr10r1420.srdf", {"name": "kuka_kr10r1420"})\
                                    .to_dict()

ROBOT_CONFIG = { 
    **ROBOT_CONFIG,
    "planning_scene_monitor": {
            "name": "planning_scene_monitor",
            "robot_description": "robot_description",
            "joint_state_topic": "/joint_states",
            "attached_collision_object_topic": "/moveit_cpp/planning_scene_monitor",
            "publish_planning_scene_topic": "/moveit_cpp/publish_planning_scene",
            "monitored_planning_scene_topic": "/moveit_cpp/monitored_planning_scene",
            "wait_for_initial_state_timeout": 10.0,
        },
        "planning_pipelines": {
            "pipeline_names": ["ompl"]
        },
        "plan_request_params": {
            "planning_attempts": 10,
            "planning_pipeline": "ompl",
            "planning_time": 5.0,
            "max_velocity_scaling_factor": 1.0,
            "max_acceleration_scaling_factor": 1.0
        },
        "ompl": {
            "planning_plugins": ["ompl_interface/OMPLPlanner"],
            "request_adapters": ["default_planning_request_adapters/ResolveConstraintFrames",
                            "default_planning_request_adapters/ValidateWorkspaceBounds",
                            "default_planning_request_adapters/CheckStartStateBounds",
                            "default_planning_request_adapters/CheckStartStateCollision"],
            "response_adapters": ["default_planning_response_adapters/AddTimeOptimalParameterization",
                             "default_planning_response_adapters/ValidateSolution",
                             "default_planning_response_adapters/DisplayMotionPath"],
            "start_state_max_bounds_error": 0.1
        }
}

class CommanderNode(Node):
    def __init__(self):
        super().__init__("commander")
        self.robot_ = MoveItPy(node_name="moveit_py", config_dict=ROBOT_CONFIG)
        self.arm_: PlanningComponent = self.robot_.get_planning_component("arm_group")

        self.pose_cmd_sub_ = self.create_subscription(
            PoseCommand, "pose_command", self.pose_command_callback, 10)

        self.named_target_cmd_sub_ = self.create_subscription(
            String, "named_target_command", self.go_to_target_callback, 10)

        self.get_current_end_effector_pose()        

    def go_to_named_target(self, name):
        self.arm_.set_start_state_to_current_state()
        self.arm_.set_goal_state(configuration_name=name)
        self.plan_and_execute(self.arm_)

    def get_current_end_effector_pose(self):
        planning_scene_monitor = self.robot_.get_planning_scene_monitor()

        with planning_scene_monitor.read_only() as scene:
            robot_state = scene.current_state
            end_effector_link = "lidar_link"
            current_pose: Pose = robot_state.get_pose(end_effector_link)

            self.get_logger().info(f"{current_pose.position}")

            return current_pose


    def go_to_pose_target(self, x, y, z, roll, pitch, yaw):
        q_x, q_y, q_z, q_w = tf_transformations.quaternion_from_euler(roll, pitch, yaw)
        pose_goal = PoseStamped()
        pose_goal.header.frame_id = "base_link"
        pose_goal.pose.position.x = x
        pose_goal.pose.position.y = y
        pose_goal.pose.position.z = z
        pose_goal.pose.orientation.x = q_x
        pose_goal.pose.orientation.y = q_y
        pose_goal.pose.orientation.z = q_z
        pose_goal.pose.orientation.w = q_w

        self.arm_.set_start_state_to_current_state()
        self.arm_.set_goal_state(pose_stamped_msg=pose_goal, pose_link="lidar_link")
        self.plan_and_execute(self.arm_)


    def plan_and_execute(self, interface):
        plan_result = interface.plan()
        if plan_result:
            self.robot_.execute(plan_result.trajectory, controllers=[])


    def pose_command_callback(self, msg: PoseCommand):
        self.go_to_pose_target(msg.x, msg.y, msg.z, msg.roll, msg.pitch, msg.yaw)

    def go_to_target_callback(self, msg: String):
        self.go_to_named_target(msg.data)


def main(args=None):
    rclpy.init(args=args)
    node = CommanderNode()
    rclpy.spin(node)
    rclpy.shutdown()