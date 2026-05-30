import rclpy
from rclpy.node import Node


class RobotActionsNode(Node):
    """Placeholder node for future robot action primitives."""

    def __init__(self) -> None:
        super().__init__("pai_robot_actions")
        self.get_logger().info("pai_robot_actions placeholder started")


def main(args=None) -> None:
    rclpy.init(args=args)
    node = RobotActionsNode()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()
