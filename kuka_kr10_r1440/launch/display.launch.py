from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_path
from launch_ros.substitutions import FindPackageShare
from launch_ros.parameter_descriptions import ParameterValue
from launch.substitutions import Command, PathJoinSubstitution
import os

def generate_launch_description():
    pkg_share = get_package_share_path('kuka_kr10_r1440')
    urdf_path = os.path.join(pkg_share, 'urdf', 'kuka_kr10_r1440.xacro')
    robot_description = ParameterValue(Command(['xacro ', urdf_path]), value_type=str)
    rviz_config_path = os.path.join(pkg_share, 'rviz', 'display_config.rviz')


    return LaunchDescription([
        Node(
            package='joint_state_publisher_gui',
            executable='joint_state_publisher_gui',
        ),
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            parameters=[{'robot_description': robot_description}]
        ),
        Node(
            package='rviz2',
            executable='rviz2',
            arguments=['-d', rviz_config_path]
        )
    ])
