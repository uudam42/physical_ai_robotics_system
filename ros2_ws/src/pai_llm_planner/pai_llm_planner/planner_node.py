import rclpy
from rclpy.node import Node


class PlannerNode(Node):
    """Placeholder node for future natural-language task planning."""

    def __init__(self) -> None:
        super().__init__("pai_llm_planner")
        self.get_logger().info("pai_llm_planner placeholder started")


def main(args=None) -> None:
    rclpy.init(args=args)
    node = PlannerNode()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()
