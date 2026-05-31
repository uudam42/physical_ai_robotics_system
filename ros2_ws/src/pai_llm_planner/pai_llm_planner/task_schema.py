ALLOWED_TASK_TYPES = {"pick_and_place", "unsupported"}

ALLOWED_STEPS = {
    "check_object",
    "move_to_pre_grasp",
    "open_gripper",
    "move_to_grasp",
    "close_gripper",
    "move_to_target",
    "return_home",
}

REQUIRED_FIELDS = {
    "task_id",
    "task_type",
    "object_name",
    "source_location",
    "target_location",
    "steps",
}


def validate_task_plan(plan: dict) -> tuple[bool, str]:
    # This validator is the safety gate between planner output and ROS 2 execution.
    # Invalid task types or hallucinated steps must never reach /task_plan.
    if not isinstance(plan, dict):
        return False, "Task plan must be a dict"

    missing_fields = sorted(REQUIRED_FIELDS - plan.keys())
    if missing_fields:
        return False, f"Missing required fields: {', '.join(missing_fields)}"

    task_type = plan["task_type"]
    if task_type not in ALLOWED_TASK_TYPES:
        return False, f"Unsupported task_type: {task_type}"

    steps = plan["steps"]
    if not isinstance(steps, list):
        return False, "Field 'steps' must be a list"

    invalid_steps = [
        str(step) for step in steps if not isinstance(step, str) or step not in ALLOWED_STEPS
    ]
    if invalid_steps:
        return False, f"Invalid steps: {', '.join(invalid_steps)}"

    if task_type == "unsupported" and steps:
        return False, "Unsupported tasks must have empty steps"

    if task_type == "pick_and_place" and not steps:
        return False, "pick_and_place tasks must have non-empty steps"

    return True, "Task plan is valid"
