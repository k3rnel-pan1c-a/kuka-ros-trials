import rclpy
from rclpy.node import Node
from sensor_msgs.msg import LaserScan, JointState, PointCloud2
from sensor_msgs_py import point_cloud2
from std_srvs.srv import Trigger
import tf2_ros
import numpy as np
import os
from datetime import datetime
from scipy.spatial.transform import Rotation


class SimpleLaserToPointCloud(Node):
    def __init__(self):
        super().__init__('simple_laser_to_pointcloud')
        self.pc_pub = self.create_publisher(PointCloud2, 'pointcloud', 10)
        self.create_subscription(JointState, 'joint_states', self.rotating_joint_cb, 10)
        self.create_subscription(LaserScan, '/scan', self.laser_cb, 10)

        self.save_srv = self.create_service(
            Trigger, 'save_point_cloud_txt', self.save_points_callback
        )

        # TF
        self.tf_buffer = tf2_ros.Buffer()
        self.tf_listener = tf2_ros.TransformListener(self.tf_buffer, self)

        self.current_angle = 0.0
        self.points = []
        self.home_tf = None  # store home pose transform

        # Frames
        self.parent_frame = 'base_link'
        self.scanner_frame = 'lidar_link'

    # ---- TF utils ----
    @staticmethod
    def tf_to_matrix(tf):
        """Convert geometry_msgs/TransformStamped → 4x4 matrix."""
        t = tf.transform.translation
        q = tf.transform.rotation

        # Translation
        mat = np.eye(4, dtype=float)
        mat[0:3, 3] = [t.x, t.y, t.z]

        # Rotation
        rot = Rotation.from_quat([q.x, q.y, q.z, q.w]).as_matrix()
        mat[0:3, 0:3] = rot
        return mat

    def get_variation_matrix(self):
        """Compute delta T = inv(home) * current"""
        try:
            transform = self.tf_buffer.lookup_transform(
                self.parent_frame,
                self.scanner_frame,
                rclpy.time.Time(),
                timeout=rclpy.duration.Duration(seconds=0.01),
            )
            if self.home_tf is None:
                self.home_tf = transform
                self.get_logger().info("Stored home pose of lidar.")
                return np.eye(4)

            home_mat = self.tf_to_matrix(self.home_tf)
            curr_mat = self.tf_to_matrix(transform)
            rel_mat = np.linalg.inv(home_mat) @ curr_mat
            return rel_mat
        except tf2_ros.TransformException as ex:
            self.get_logger().warn(f"TF lookup failed: {ex}")
            return np.eye(4)

    # ---- Joint state callback ----
    def rotating_joint_cb(self, msg: JointState):
        if "cylinder_to_drill_bit" in msg.name:
            idx = msg.name.index("cylinder_to_drill_bit")
            self.current_angle = msg.position[idx]

    # ---- Laser to PointCloud ----
    def laser_cb(self, msg: LaserScan):
        try:
            # Direct transform from scanner to base at the scan timestamp 
            transform = self.tf_buffer.lookup_transform(
                self.parent_frame,
                self.scanner_frame,
                msg.header.stamp,
                timeout=rclpy.duration.Duration(seconds=0.01),
            )
            tf_mat = self.tf_to_matrix(transform)

            points = []
            angle = msg.angle_min
            for r in msg.ranges:
                if r is None or np.isnan(r) or r == float('inf'):
                    angle += msg.angle_increment
                    continue
                if r < msg.range_min or r > msg.range_max:
                    angle += msg.angle_increment
                    continue

                # Point in scanner frame
                x = r * np.cos(angle)
                y = 0.0
                z = r * np.sin(angle)

                # Transform directly into base frame
                vec = np.array([x, y, z, 1.0])
                tx, ty, tz, _ = tf_mat @ vec
                points.append((tx, ty, tz))
                angle += msg.angle_increment

            # Append new points to accumulated list
            self.points.extend(points)

            # Publish accumulated cloud. Keep the original scan timestamp so
            # RViz applies the same TF when visualizing.
            header = msg.header
            header.frame_id = self.parent_frame
            header.stamp = msg.header.stamp
            cloud_msg = point_cloud2.create_cloud_xyz32(header, self.points)
            self.pc_pub.publish(cloud_msg)

        except Exception as ex:
            self.get_logger().warn(f"PointCloud conversion failed: {ex}")

    # ---- Save points ----
    def save_points_callback(self, request, response):
        try:
            if not self.points:
                response.success = False
                response.message = "No points to save"
                return response

            save_dir = os.path.expanduser("~/pointcloud_data")
            os.makedirs(save_dir, exist_ok=True)
            timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
            filepath = os.path.join(save_dir, f"pointcloud_{timestamp}.xyz")

            with open(filepath, 'w') as f:
                for p in self.points:
                    f.write(f"{p[0]:.6f} {p[1]:.6f} {p[2]:.6f}\n")

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
    node = SimpleLaserToPointCloud()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
