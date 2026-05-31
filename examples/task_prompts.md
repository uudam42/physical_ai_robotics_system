# Task Prompt Examples

The planner accepts natural language commands and produces a task-plan dictionary before converting it into a typed ROS 2 `TaskPlan` message.

## Supported Commands

```text
pick up the red cube and place it into the blue box
```

```text
move the red cube from the table to the blue box
```

```text
place the cube into the box
```

Supported commands map to `task_type="pick_and_place"` and use only whitelisted task steps.

## Unsupported Commands

```text
drive to the kitchen
```

```text
fly the drone outside
```

```text
cut the object in half
```

```text
dance around the room
```

Unsupported commands map to `task_type="unsupported"` with empty `steps`.

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

## Expected Pick-And-Place Shape

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
