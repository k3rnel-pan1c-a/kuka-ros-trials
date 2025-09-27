import rclpy
from rclpy.node import Node
from sensor_msgs.msg import LaserScan, JointState, PointCloud2
from sensor_msgs_py import point_cloud2
import numpy as np
import tf2_ros
import tf2_geometry_msgs
from geometry_msgs.msg import TransformStamped, PointStamped
from tf2_ros import TransformException
from std_srvs.srv import Trigger
import os
from datetime import datetime
from std_srvs.srv import Trigger
import os
from datetime import datetime

class RotaryPointCloud(Node):
    def __init__(self):
        super().__init__('rotary_pointcloud')
        self.pc_pub = self.create_publisher(PointCloud2, 'pointcloud', 10)
        self.create_subscription(JointState, 'joint_states', self.joint_cb, 10)
        self.create_subscription(LaserScan, '/scan', self.laser_cb, 10)

        # Add service to save points
        self.save_srv = self.create_service(Trigger, 'save_pointcloud_txt', self.save_points_callback)
        
        # Create service to save points to text file
        self.save_srv = self.create_service(
            Trigger, 
            'save_pointcloud_txt', 
            self.save_points_callback
        )

        # TF setup
        self.tf_buffer = tf2_ros.Buffer()
        self.tf_listener = tf2_ros.TransformListener(self.tf_buffer, self)
        
        # Your existing parameters
        self.current_angle = 0.0
        self.points = [] 
        self.distance_to_centre = None  # calculated from tf
        
        # Frame names
        self.target_frame = 'cylinder_link'
        self.scanner_frame = 'lidar_link'
        
        # Calculate initial distance 
        self.update_distance_to_centre()

        # vertical angles from urdf
        self.vertical_angles = np.linspace(0.3735, 0.4363, 640)  

    def update_distance_to_centre(self):
        try:
            # Look up transform between cylinder and lidar
            transform = self.tf_buffer.lookup_transform(
                self.target_frame,
                self.scanner_frame,
                rclpy.time.Time(),
                timeout=rclpy.duration.Duration(seconds=1.0)
            )

            temp_x = transform.transform.translation.x
            temp_y = transform.transform.translation.y
            
            self.distance_to_centre = np.sqrt(temp_x ** 2 + temp_y ** 2)
            self.get_logger().info(f'Updated distance to centre: {self.distance_to_centre}')
        except TransformException as ex:
            self.get_logger().warn(f'Could not get distance to centre: {ex}')
            if self.distance_to_centre is None:
                pass
    
    def joint_cb(self, msg: JointState):
        if "cylinder_to_drill_bit" in msg.name:
            idx = msg.name.index("cylinder_to_drill_bit")
            self.current_angle = msg.position[idx]
            # Update distance when joint moves
            self.update_distance_to_centre()

    def laser_cb(self, msg: LaserScan):
        try:
            # Get transform from scanner to base frame
            transform = self.tf_buffer.lookup_transform(
                self.target_frame,
                msg.header.frame_id,
                msg.header.stamp,
                timeout=rclpy.duration.Duration(seconds=0.1)
            )

            # Get all ranges from lidar
            ranges = np.array(msg.ranges)
            
            # Create meshgrid for ranges and vertical angles
            ranges_mesh, angles_mesh = np.meshgrid(ranges, self.vertical_angles)
            
            # Create points in laser profiler's local frame
            scanner_X = ranges_mesh * np.cos(angles_mesh)  # Forward distance
            scanner_Y = np.zeros_like(ranges_mesh)         # No horizontal distance component
            scanner_Z = ranges_mesh * np.sin(angles_mesh)  # Vertical component
            
            # Flatten arrays
            scanner_X = scanner_X.flatten()
            scanner_Y = scanner_Y.flatten()
            scanner_Z = scanner_Z.flatten()
            
            # Remove any invalid points
            valid_points = np.isfinite(scanner_X) & np.isfinite(scanner_Y) & np.isfinite(scanner_Z)
            scanner_X = scanner_X[valid_points]
            scanner_Y = scanner_Y[valid_points]
            scanner_Z = scanner_Z[valid_points]

            # Transform points to fixed frame first
            pre_rotation_points = []
            for x, y, z in zip(scanner_X, scanner_Y, scanner_Z):
                p = PointStamped()
                p.header = msg.header
                p.point.x = float(x)
                p.point.y = float(y)
                p.point.z = float(z)
                
                # Transform point from scanner to fixed frame
                transformed_p = tf2_geometry_msgs.do_transform_point(p, transform)
                pre_rotation_points.append([
                    transformed_p.point.x,
                    transformed_p.point.y,
                    transformed_p.point.z
                ])
            
            # apply rotary motion to transformed points
            transformed_points = []
            for point in pre_rotation_points:
                # Calculate radius from center of rotation
                x, y, z = point
                radius = self.np.sqrt(x**2 + y**2)  # Distance from center in XY plane
                
                # Apply rotation
                rotated_x = radius * np.cos(self.current_angle)
                rotated_y = -radius * np.sin(self.current_angle)
                
                # Keep the same height
                transformed_points.append([rotated_x, rotated_y, z])          
            
            # Append transformed points
            self.points.extend(transformed_points)

            # Create and publish pointcloud
            header = msg.header
            header.frame_id = self.target_frame  # Set to base frame
            cloud_msg = point_cloud2.create_cloud_xyz32(header, self.points)
            self.pc_pub.publish(cloud_msg)

        except TransformException as ex:
            self.get_logger().warn(f'Could not transform point cloud: {ex}')

    def save_points_callback(self, request, response):
        try:
            if not self.points:
                response.success = False
                response.message = "No points to save"
                return response

            # Create directory if it doesn't exist
            save_dir = os.path.expanduser("~/pointcloud_data")
            os.makedirs(save_dir, exist_ok=True)

            # Create filename with timestamp
            timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
            filename = f"pointcloud_{timestamp}.xyz"
            filepath = os.path.join(save_dir, filename)

            # Save points to text file
            with open(filepath, 'w') as f:
                for point in self.points:
                    f.write(f"{point[0]:.6f} {point[1]:.6f} {point[2]:.6f}\n")

            self.get_logger().info(f"Saved {len(self.points)} points to {filepath}")
            response.success = True
            response.message = f"Saved points to {filepath}"
            return response

        except Exception as e:
            response.success = False
            response.message = f"Failed to save points: {str(e)}"
            return response

def main():
    rclpy.init()
    node = RotaryPointCloud()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
