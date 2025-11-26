from launch import LaunchDescription
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from launch.substitutions import PathJoinSubstitution

def generate_launch_description():
    pkg_share = FindPackageShare('kuka_kr10_r1440').find('kuka_kr10_r1440')

    urdf_path = PathJoinSubstitution([
        pkg_share,
        'urdf',
        'kuka_kr10_r1440.urdf'
    ])

    rviz_config = PathJoinSubstitution([
        pkg_share,
        'urdf.rviz'
    ])

    return LaunchDescription([
        Node(
            package='joint_state_publisher_gui',
            executable='joint_state_publisher_gui',
            name='joint_state_publisher_gui'
        ),
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            name='robot_state_publisher',
            parameters=[{'robot_description': open(f'{pkg_share}/urdf/kuka_kr10_r1440.urdf').read()}]
        ),
        Node(
            package='rviz2',
            executable='rviz2',
            name='rviz2',
            arguments=['-d', f'{pkg_share}/urdf.rviz']
        )
    ])
