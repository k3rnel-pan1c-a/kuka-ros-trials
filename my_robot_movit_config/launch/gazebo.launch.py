import os
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
from launch.substitutions import Command, FindExecutable, PathJoinSubstitution, LaunchConfiguration
from launch_ros.actions import SetParameter
from launch_ros.substitutions import FindPackageShare
from launch.conditions import IfCondition
from launch.actions import ExecuteProcess, SetEnvironmentVariable
from ament_index_python.packages import get_package_share_path

def generate_launch_description():
    package_path = get_package_share_path('my_robot_description')
    # urdf_path = os.path.join(package_path, 'urdf', 'my_robot.urdf.xacro')
    package_parent = str(package_path.parent)
    ros_gz_bridge_config_path = os.path.join(package_path, 'config', 'ros_gz_bridge.yaml')
    set_gz_resource_path = SetEnvironmentVariable(
    name='GZ_SIM_RESOURCE_PATH',
    value=f"{package_parent}:{package_path}"
    )

    pkg_share = FindPackageShare('my_robot_movit_config')

    # Static virtual joint TFs (if present)
    static_virtual_joints_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([
                pkg_share, 'launch', 'static_virtual_joint_tfs.launch.py'
            ])
        )
    )

    # Robot State Publisher
    rsp_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([
                pkg_share, 'launch', 'rsp.launch.py'
            ])
        )
    )

    # Gazebo (Ignition/ros_gz_sim) launch
    from launch.actions import ExecuteProcess
    gazebo_launch = ExecuteProcess(
        cmd=['ros2', 'launch', 'ros_gz_sim', 'gz_sim.launch.py', 'gz_args:=empty.sdf -r'],
        output='screen'
    )

    # Spawn robot in Gazebo (Ignition/ros_gz_sim)
    spawn_entity = Node(
        package='ros_gz_sim',
        executable='create',
        arguments=['-topic', 'robot_description'],
        output='screen'
    )

    ros_gz_bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        parameters=[{'config_file': ros_gz_bridge_config_path}],
    )

    # ros2_control controllers
    controllers_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([
                pkg_share, 'launch', 'spawn_controllers.launch.py'
            ])
        )
    )

    # MoveIt move_group
    move_group_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([
                pkg_share, 'launch', 'move_group.launch.py'
            ])
        )
    )

    # RViz with MoveIt Motion Planning plugin
    rviz_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([
                pkg_share, 'launch', 'moveit_rviz.launch.py'
            ])
        ),
    )


    my_robot_commander_cpp = Node(
        package = 'my_robot_commander_cpp',
        executable = 'test_moveit'
    )


    return LaunchDescription([
        SetParameter(name='use_sim_time', value=True),
        set_gz_resource_path,
        static_virtual_joints_launch,
        rsp_launch,
        gazebo_launch,
        spawn_entity,
        ros_gz_bridge,
        controllers_launch,
        move_group_launch,
        rviz_launch,
        my_robot_commander_cpp
    ])