# Physical AI Architecture

This project separates natural language planning from deterministic robot execution.

```text
Natural language command
-> pai_llm_planner
-> planner_mode mock/openai
-> schema validation
-> pai_task_msgs/TaskPlan
-> C++ BehaviorTree.CPP executor
-> /robot_action service
-> MoveIt-ready MotionPlanner / GripperController
-> pai_task_msgs/TaskStatus
```

## Language Planning

Python is used for planner and LLM integration. The `pai_llm_planner` package can run in mock mode or optional OpenAI planner mode. Both modes produce task-plan dictionaries that must pass schema validation before being converted into typed ROS 2 messages.

## Typed ROS 2 Boundary

Validated task plans are published as `pai_task_msgs/msg/TaskPlan`. This typed message boundary prevents arbitrary text from flowing directly into execution.

## BehaviorTree Execution

C++ is used for BehaviorTree.CPP execution. The C++ executor consumes typed task plans, loads BehaviorTree.CPP XML, and calls whitelisted robot action steps through `/robot_action`.

## Robot Action Service

The `/robot_action` service is implemented by `pai_moveit_interface_cpp`. It routes action requests through a MoveIt-ready `MotionPlanner` and `GripperController` abstraction.

MoveIt 2 support is optional behind `BUILD_WITH_MOVEIT=ON`. Mock fallback exists so developers can run and test the architecture without robot hardware, robot descriptions, controllers, or a MoveIt installation.
