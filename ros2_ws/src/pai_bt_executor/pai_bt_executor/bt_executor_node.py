import rclpy
from rclpy.node import Node


class BehaviorTreeExecutorNode(Node):
    """Placeholder node for future behavior tree execution."""

    def __init__(self) -> None:
        super().__init__("pai_bt_executor")
        self.get_logger().info("pai_bt_executor placeholder started")


def main(args=None) -> None:
    rclpy.init(args=args)
    node = BehaviorTreeExecutorNode()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()
