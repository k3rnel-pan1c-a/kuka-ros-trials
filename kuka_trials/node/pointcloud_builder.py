import rclpy
from rclpy.node import Node
from sensor_msgs.msg import LaserScan, JointState, PointCloud2
from sensor_msgs_py import point_cloud2
import numpy as np

class RotaryPointCloud(Node):
    def __init__(self):
        super().__init__('rotary_pointcloud')
        self.pc_pub = self.create_publisher(PointCloud2, 'pointcloud', 10)
        self.create_subscription(JointState, 'joint_states', self.joint_cb, 10)
        self.create_subscription(LaserScan, '/scan', self.laser_cb, 10)

        self.current_angle = 0.0
        self.points = [] 
        self.distance_to_centre = 0.464 

        # vertical angles from urdf
        self.vertical_angles = np.linspace(0.3735, 0.4363, 640)  

    def joint_cb(self, msg: JointState):
        if "cylinder_to_drill_bit" in msg.name:
            idx = msg.name.index("cylinder_to_drill_bit")
            self.current_angle = msg.position[idx]

    def laser_cb(self, msg: LaserScan):
        # single range from lidar
        r = msg.ranges[0]  
        radius = self.distance_to_centre - r

        # apply transformation to generate pointcloud
        X =  radius * np.cos(self.current_angle)
        Y =  -radius * np.sin(self.current_angle)
        Z = r * np.sin(self.vertical_angles)

        # append points
        points = np.stack([X*np.ones_like(Z), Y*np.ones_like(Z), Z], axis=-1)
        self.points.extend(points.tolist())

        # publish pointcloud
        cloud_msg = point_cloud2.create_cloud_xyz32(msg.header, self.points)
        self.pc_pub.publish(cloud_msg)

def main():
    rclpy.init()
    node = RotaryPointCloud()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
