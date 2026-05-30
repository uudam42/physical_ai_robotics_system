import rclpy
from rclpy.node import Node


class MoveItInterfaceNode(Node):
    """Placeholder node for future MoveIt 2 integration."""

    def __init__(self) -> None:
        super().__init__("pai_moveit_interface")
        self.get_logger().info("pai_moveit_interface placeholder started")


def main(args=None) -> None:
    rclpy.init(args=args)
    node = MoveItInterfaceNode()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()
