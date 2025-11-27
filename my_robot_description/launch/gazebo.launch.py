from launch import LaunchDescription
from ament_index_python.packages import get_package_share_path
from launch_ros.parameter_descriptions import ParameterValue
from launch_ros.actions import Node
from launch.actions import ExecuteProcess, SetEnvironmentVariable
from launch.substitutions import Command, EnvironmentVariable
import os

def generate_launch_description():
    package_path = get_package_share_path('my_robot_description')
    urdf_path = os.path.join(package_path, 'urdf', 'my_robot.urdf.xacro')
    ros_gz_bridge_config_path = os.path.join(package_path, 'config', 'ros_gz_bridge.yaml')
    
    robot_description = ParameterValue(Command(['xacro ', urdf_path]), value_type=str)

    # Get the parent directory (install/share) to help Gazebo find packages
    package_parent = str(package_path.parent)
    
    set_gz_resource_path = SetEnvironmentVariable(
        name='GZ_SIM_RESOURCE_PATH',
        value=f"{package_parent}:{package_path}"
    )

    robot_state_publisher_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description': robot_description}],
    )

    gz_sim_process = ExecuteProcess(
        cmd=['ros2', 'launch', 'ros_gz_sim', 'gz_sim.launch.py', 'gz_args:=empty.sdf -r'],
        output='screen'
    )

    ros_gz_bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        parameters=[{'config_file': ros_gz_bridge_config_path}],
    )

    spawn_entity = Node(
        package='ros_gz_sim',
        executable='create',
        arguments=[
            '-topic', 'robot_description'],
        output='screen'
    )

    return LaunchDescription([
        set_gz_resource_path,
        gz_sim_process,
        robot_state_publisher_node,
        spawn_entity,
        ros_gz_bridge
    ])
