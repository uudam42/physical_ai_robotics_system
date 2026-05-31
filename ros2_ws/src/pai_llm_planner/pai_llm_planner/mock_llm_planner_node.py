from pai_llm_planner.planners import MockPlanner
from pai_llm_planner.task_plan_builder import (
    build_rejected_task_plan,
    build_task_plan_msg,
)
from pai_llm_planner.task_schema import validate_task_plan
from pai_task_msgs.msg import TaskPlan
import rclpy
from rclpy.node import Node
from std_msgs.msg import String


class MockLlmPlannerNode(Node):
    """Mock natural-language planner that emits typed task plans."""

    def __init__(self) -> None:
        super().__init__("mock_llm_planner_node")
        self.planner = MockPlanner()
        self.task_plan_publisher = self.create_publisher(TaskPlan, "/task_plan", 10)
        self.user_command_subscription = self.create_subscription(
            String,
            "/user_command",
            self.handle_user_command,
            10,
        )
        self.get_logger().info("Mock LLM planner node started")
        self.get_logger().info("Listening on /user_command and publishing to /task_plan")

    def handle_user_command(self, msg: String) -> None:
        command = msg.data.strip()
        self.get_logger().info(f"Received user command: {command}")

        plan = self.create_task_plan(command)
        self.task_plan_publisher.publish(plan)

        self.get_logger().info(
            f"Published task plan '{plan.task_id}' with type '{plan.task_type}'"
        )

    def create_task_plan(self, command: str) -> TaskPlan:
        plan_dict = self.planner.plan(command)
        is_valid, validation_message = validate_task_plan(plan_dict)

        if is_valid:
            if plan_dict["task_type"] == "pick_and_place":
                self.get_logger().info("Command matched mock pick-and-place planner rule")
            else:
                self.get_logger().warn("Command is unsupported; publishing rejected task plan")
            return build_task_plan_msg(plan_dict)

        self.get_logger().error(f"Planner output failed validation: {validation_message}")
        return build_rejected_task_plan(validation_message)


def main(args=None) -> None:
    rclpy.init(args=args)
    node = MockLlmPlannerNode()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()
