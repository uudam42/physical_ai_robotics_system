from pai_task_msgs.msg import TaskPlan, TaskStep


def build_task_plan_msg(plan: dict) -> TaskPlan:
    return TaskPlan(
        task_id=plan["task_id"],
        task_type=plan["task_type"],
        object_name=plan["object_name"],
        source_location=plan["source_location"],
        target_location=plan["target_location"],
        steps=[
            TaskStep(name=step, status="pending", parameters_json="{}")
            for step in plan["steps"]
        ],
    )


def build_rejected_task_plan(reason: str) -> TaskPlan:
    return TaskPlan(
        task_id="rejected_task",
        task_type="unsupported",
        object_name="",
        source_location="",
        target_location="",
        steps=[],
    )
