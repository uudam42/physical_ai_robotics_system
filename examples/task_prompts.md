# Task Prompt Examples

The planner accepts natural language commands and produces a task-plan dictionary before converting it into a typed ROS 2 `TaskPlan` message.

## Supported Commands

```text
Pick up the red cube and place it into the blue box.
```

```text
Please pick the cube from the table and put it in the box.
```

These commands should produce a `pick_and_place` task using only the allowed steps.

## Unsupported Commands

```text
Open the kitchen drawer.
```

```text
Drive to the charging dock.
```

Unsupported commands should produce:

```json
{
  "task_id": "rejected_task",
  "task_type": "unsupported",
  "object_name": "",
  "source_location": "",
  "target_location": "",
  "steps": []
}
```

## Expected JSON Shape

```json
{
  "task_id": "demo_task_001",
  "task_type": "pick_and_place",
  "object_name": "red_cube",
  "source_location": "table",
  "target_location": "blue_box",
  "steps": [
    "check_object",
    "move_to_pre_grasp",
    "open_gripper",
    "move_to_grasp",
    "close_gripper",
    "move_to_target",
    "open_gripper",
    "return_home"
  ]
}
```

LLM output is always validated before it is converted into a ROS 2 `TaskPlan` message and published for execution.
