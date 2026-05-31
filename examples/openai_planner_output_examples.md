# OpenAI Planner Output Examples

These JSON examples document the expected planner output shape. Actual ROS publication uses typed `pai_task_msgs/msg/TaskPlan` messages after validation.

## Supported Pick-And-Place Command

Command:

```text
pick up the red cube and place it into the blue box
```

Expected LLM output:

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

## Unsupported Command

Command:

```text
fly the drone outside
```

Expected LLM output:

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

## Invalid LLM Output

```json
{
  "task_id": "unsafe_task",
  "task_type": "pick_and_place",
  "object_name": "red_cube",
  "source_location": "table",
  "target_location": "blue_box",
  "steps": [
    "check_object",
    "throw_object"
  ]
}
```

Validation rejects this plan because `throw_object` is not an allowed step. The planner node publishes a rejected `TaskPlan` instead of forwarding the invalid action to execution.
