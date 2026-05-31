# Architecture Diagram

```mermaid
flowchart TD
    A[Natural Language Command] --> B[LLM Planner / Mock Planner]
    B --> C[Schema Validation]
    C --> D[pai_task_msgs/msg/TaskPlan]
    D --> E[C++ BehaviorTree.CPP Executor]
    E --> F[/robot_action Service]
    F --> G[MotionPlanner / GripperController]
    G --> H[MoveIt-ready Interface]
    E --> I[pai_task_msgs/msg/TaskStatus]
    F --> I
```

## Layers

- Natural Language Command: user-facing instruction such as "pick up the red cube and place it into the blue box".
- LLM Planner / Mock Planner: Python planner layer in `pai_llm_planner`, selected with `planner_mode`.
- Schema Validation: safety gate that rejects unsupported task types and hallucinated steps.
- `TaskPlan`: typed ROS 2 message consumed by execution.
- C++ BehaviorTree.CPP Executor: deterministic task orchestration in `pai_bt_executor_cpp`.
- `/robot_action` Service: stable service boundary for robot action requests.
- MotionPlanner / GripperController: MoveIt-ready action implementations with mock fallback.
- MoveIt-ready Interface: optional future integration point for robot-specific MoveIt 2 configuration.
- `TaskStatus`: typed ROS 2 status stream for execution monitoring.
