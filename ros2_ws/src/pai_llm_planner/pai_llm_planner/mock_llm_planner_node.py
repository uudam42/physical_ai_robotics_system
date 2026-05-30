from pai_task_msgs.msg import TaskPlan, TaskStep
import rclpy
from rclpy.node import Node
from std_msgs.msg import String


PICK_AND_PLACE_STEPS = [
    "check_object",
    "move_to_pre_grasp",
    "open_gripper",
    "move_to_grasp",
    "close_gripper",
    "move_to_target",
    "open_gripper",
    "return_home",
]


class MockLlmPlannerNode(Node):
    """Mock natural-language planner that emits typed task plans."""

    def __init__(self) -> None:
        super().__init__("mock_llm_planner_node")
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
        normalized_command = command.lower()
        required_words = ("pick", "cube", "box")

        if all(word in normalized_command for word in required_words):
            self.get_logger().info("Command matched mock pick-and-place planner rule")
            return TaskPlan(
                task_id="demo_task_001",
                task_type="pick_and_place",
                object_name="red_cube",
                source_location="table",
                target_location="blue_box",
                steps=[
                    TaskStep(name=step, status="pending", parameters_json="{}")
                    for step in PICK_AND_PLACE_STEPS
                ],
            )

        self.get_logger().warn("Command is unsupported; publishing rejected task plan")
        return TaskPlan(
            task_id="rejected_task",
            task_type="unsupported",
            object_name="",
            source_location="",
            target_location="",
            steps=[],
        )


def main(args=None) -> None:
    rclpy.init(args=args)
    node = MockLlmPlannerNode()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()
