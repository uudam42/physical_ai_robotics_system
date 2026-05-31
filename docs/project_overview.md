# Project Overview

Physical AI Robot Task Executor translates natural language robot instructions into validated task plans, executes them through BehaviorTree.CPP, and routes robot actions through a MoveIt-ready service layer.

## What The Project Does

The system accepts a command such as:

```text
pick up the red cube and place it into the blue box
```

It converts that command into a typed ROS 2 `TaskPlan`, validates the plan against a whitelist of task types and steps, executes the task through a C++ BehaviorTree.CPP executor, and reports progress through typed `TaskStatus` messages.

## Why It Is Physical AI

The project connects language-level intent to robotics execution while preserving safety boundaries. It treats LLM output as a suggestion, not a command stream. Execution is performed by typed ROS 2 interfaces, BehaviorTree.CPP, and a robot action service.

## Python Components

Python is used where iteration speed matters:

- `pai_llm_planner`: mock planner and optional OpenAI planner mode.
- `task_schema.py`: validation safety gate for planner outputs.
- `task_plan_builder.py`: conversion from validated dictionaries to typed ROS 2 task messages.

## C++ Components

C++ is used for execution and robot-facing layers:

- `pai_bt_executor_cpp`: BehaviorTree.CPP executor for deterministic task orchestration.
- `pai_moveit_interface_cpp`: `/robot_action` service, `MotionPlanner`, and `GripperController`.

## ROS 2 Messages And Services

The project uses custom interfaces in `pai_task_msgs`:

- `TaskPlan`: typed task plan sent from planner to executor.
- `TaskStep`: typed task step inside a task plan.
- `TaskStatus`: execution status stream.
- `RobotAction`: action description message.
- `ExecuteRobotAction`: service contract for `/robot_action`.

## BehaviorTree.CPP

BehaviorTree.CPP provides structured execution. The current tree is stored in `bt_xml/pick_and_place.xml` and maps high-level task steps to robot action service calls.

## MoveIt 2 Preparation

MoveIt 2 support is prepared but optional. The default build uses mock fallback and does not require MoveIt 2. The optional `BUILD_WITH_MOVEIT=ON` path enables MoveIt-related compile guards for future integration.

Placeholder target names and poses live in `configs/moveit_targets.yaml`.

## Validation Before Execution

LLM outputs are validated before they become ROS 2 `TaskPlan` messages. Unsupported task types, non-list steps, and hallucinated action names are rejected. This prevents arbitrary LLM output from reaching BehaviorTree.CPP execution or robot action services.
