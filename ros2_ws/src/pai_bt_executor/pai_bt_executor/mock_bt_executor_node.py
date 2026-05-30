import json
from json import JSONDecodeError

import rclpy
from rclpy.node import Node
from std_msgs.msg import String


class MockBehaviorTreeExecutorNode(Node):
    """Mock behavior tree executor that logs JSON task plan steps."""

    def __init__(self) -> None:
        super().__init__("mock_bt_executor_node")
        self.task_plan_subscription = self.create_subscription(
            String,
            "/task_plan",
            self.handle_task_plan,
            10,
        )
        self.get_logger().info("Mock behavior tree executor node started")
        self.get_logger().info("Listening on /task_plan")

    def handle_task_plan(self, msg: String) -> None:
        try:
            task_plan = json.loads(msg.data)
        except JSONDecodeError as exc:
            self.get_logger().error(f"Failed to parse task plan JSON: {exc}")
            return

        task_type = task_plan.get("task_type", "")
        if task_type == "pick_and_place":
            self.execute_pick_and_place(task_plan)
            return

        if task_type == "unsupported":
            task_id = task_plan.get("task_id", "unknown")
            self.get_logger().warn(f"Task '{task_id}' is unsupported; skipping execution")
            return

        self.get_logger().warn(f"Unknown task_type '{task_type}'; skipping execution")

    def execute_pick_and_place(self, task_plan: dict) -> None:
        task_id = task_plan.get("task_id", "unknown")
        object_name = task_plan.get("object_name", "unknown")
        target_location = task_plan.get("target_location", "unknown")
        steps = task_plan.get("steps", [])

        self.get_logger().info(f"Executing task_id: {task_id}")
        self.get_logger().info(f"Object: {object_name}")
        self.get_logger().info(f"Target location: {target_location}")

        for index, step in enumerate(steps, start=1):
            self.get_logger().info(f"Step {index}: {step}")


def main(args=None) -> None:
    rclpy.init(args=args)
    node = MockBehaviorTreeExecutorNode()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()
