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
    robot_description = Command([
        FindExecutable(name='xacro'), ' ',
        PathJoinSubstitution([
            FindPackageShare('my_robot_description'),
            'urdf',
            'my_robot.xacro'
        ])
    ])

    publish_robot_description = ExecuteProcess(
        cmd=[
            'bash', '-lc',
            r"""python3 - <<'PY'
    import os
    import rclpy
    from rclpy.node import Node
    from rclpy.qos import QoSProfile, DurabilityPolicy, ReliabilityPolicy
    from std_msgs.msg import String

    urdf = os.environ.get("URDF", "")
    if not urdf.strip():
        raise RuntimeError("URDF env var is empty (xacro output missing)")

    rclpy.init()
    node = Node("robot_description_publisher")

    qos = QoSProfile(depth=1)
    qos.durability = DurabilityPolicy.TRANSIENT_LOCAL
    qos.reliability = ReliabilityPolicy.RELIABLE

    pub = node.create_publisher(String, "/robot_description", qos)
    msg = String()
    msg.data = urdf

    for _ in range(8):
        pub.publish(msg)
        rclpy.spin_once(node, timeout_sec=0.2)

    node.get_logger().info(f"Published /robot_description (len={len(urdf)})")
    node.destroy_node()
    rclpy.shutdown()
    PY"""
        ],
        additional_env={'URDF': robot_description},
        output='screen'
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

    #old ros2_control controllers
    # ros2_control controllers
    # controllers_launch = IncludeLaunchDescription(
    #     PythonLaunchDescriptionSource(
    #         PathJoinSubstitution([
    #             pkg_share, 'launch', 'spawn_controllers.launch.py'
    #         ])
    #     )
    # )

    #fix the switch timeout
    spawn_controllers = Node( 
        package="controller_manager",
        executable="spawner",
        arguments=[
            "joint_state_broadcaster",
            "arm_controller",
            "rotary_velocity_controller",
            "--controller-manager", "/controller_manager",
            "--controller-manager-timeout", "120",
            "--switch-timeout", "120",
            "--service-call-timeout", "120",
        ],
        output="screen",
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
        executable = 'test_moveit',
        output='screen',
        parameters=[
        {'robot_description': robot_description},
    ],)

    return LaunchDescription([
        SetParameter(name='use_sim_time', value=True),
        set_gz_resource_path,
        publish_robot_description, 
        static_virtual_joints_launch,
        rsp_launch,
        gazebo_launch,
        spawn_entity,
        ros_gz_bridge,
        #controllers_launch,
        spawn_controllers,
        move_group_launch,
        rviz_launch,
        my_robot_commander_cpp
    ])