import rclpy
from rclpy.node import Node
from sensor_msgs.msg import PointCloud2
from sensor_msgs_py import point_cloud2
from sensor_msgs.msg import JointState
import numpy as np

class RotaryPointCloud(Node):
    def __init__(self):
        super().__init__('pointcloud_builder')
        self.pc_pub = self.create_publisher(PointCloud2,'pointcloud',10)
        
        self.create_subscription(JointState,'joint_states',self.joint_cb,10)
        self.create_subscription(PointCloud2,'/scan',self.laser_cb,10)
        
        self.current_angle = 0.0
        self.points = []
        self.distance_to_centre = 0.45
        
    def joint_cb(self,msg):
        if "cylinder_to_drill_bit" in msg.name:
            idx = msg.name.index("cylinder_to_drill_bit")
            self.current_angle = msg.position[idx]
    
    def laser_cb(self,msg):
        angles = np.linspace(msg.angle_min, msg.angle_max, len(msg.ranges))
        ranges = np.array(msg.ranges)
        
        z = ranges * np.cos(angles)
        y = ranges * np.sin(angles)
        X = y * np.cos(self.current_angle)
        Y = y * np.sin(self.current_angle)
        Z = z
        
        self.points.append([X,Y,Z])
        
        cloud = point_cloud2.create_cloud_xyz32(msg.header, self.points)
        self.pc_pub.publish(cloud)

def main():
    rclpy.init()
    node = RotaryPointCloud()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
        

        