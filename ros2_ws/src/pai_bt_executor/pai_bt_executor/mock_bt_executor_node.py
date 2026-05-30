from pai_task_msgs.msg import TaskPlan, TaskStatus
import rclpy
from rclpy.node import Node


class MockBehaviorTreeExecutorNode(Node):
    """Mock behavior tree executor that logs typed task plan steps."""

    def __init__(self) -> None:
        super().__init__("mock_bt_executor_node")
        self.task_plan_subscription = self.create_subscription(
            TaskPlan,
            "/task_plan",
            self.handle_task_plan,
            10,
        )
        self.task_status_publisher = self.create_publisher(TaskStatus, "/task_status", 10)
        self.get_logger().info("Mock behavior tree executor node started")
        self.get_logger().info("Listening on /task_plan")

    def handle_task_plan(self, msg: TaskPlan) -> None:
        if msg.task_type == "pick_and_place":
            self.execute_pick_and_place(msg)
            return

        if msg.task_type == "unsupported":
            self.get_logger().warn(f"Task '{msg.task_id}' is unsupported; skipping execution")
            self.publish_status(
                task_id=msg.task_id,
                current_step="",
                status="rejected",
                message="Unsupported task type",
            )
            return

        self.get_logger().warn(f"Unknown task_type '{msg.task_type}'; skipping execution")
        self.publish_status(
            task_id=msg.task_id,
            current_step="",
            status="rejected",
            message=f"Unknown task type: {msg.task_type}",
        )

    def execute_pick_and_place(self, task_plan: TaskPlan) -> None:
        self.get_logger().info(f"Executing task_id: {task_plan.task_id}")
        self.get_logger().info(f"Object: {task_plan.object_name}")
        self.get_logger().info(f"Target location: {task_plan.target_location}")

        for index, step in enumerate(task_plan.steps, start=1):
            self.get_logger().info(f"Step {index}: {step.name}")
            self.publish_status(
                task_id=task_plan.task_id,
                current_step=step.name,
                status="running",
                message=f"Running step {index}: {step.name}",
            )
            self.publish_status(
                task_id=task_plan.task_id,
                current_step=step.name,
                status="completed",
                message=f"Completed step {index}: {step.name}",
            )

    def publish_status(
        self,
        task_id: str,
        current_step: str,
        status: str,
        message: str,
    ) -> None:
        self.task_status_publisher.publish(
            TaskStatus(
                task_id=task_id,
                current_step=current_step,
                status=status,
                message=message,
            )
        )


def main(args=None) -> None:
    rclpy.init(args=args)
    node = MockBehaviorTreeExecutorNode()
    try:
        rclpy.spin(node)
    finally:
        node.destroy_node()
        rclpy.shutdown()
