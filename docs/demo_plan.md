# Demo Plan

The first demo should prove the full intended path without requiring complex autonomy.

## Demo Scenario

User instruction:

```text
Pick up the red block and place it in the blue bin.
```

Expected high-level flow:

1. `pai_llm_planner` converts the instruction into a structured task plan.
2. `pai_bt_executor` converts the task plan into BehaviorTree.CPP XML.
3. The behavior tree calls robot action primitives.
4. `pai_robot_actions` requests motion through `pai_moveit_interface`.
5. MoveIt 2 executes the motion in simulation.

## Initial Success Criteria

- The demo can be launched from a single ROS 2 launch file.
- The task plan is inspectable before execution.
- The generated behavior tree XML is saved for debugging.
- Each task step reports started, succeeded, or failed.
- The simulated robot completes a basic pick-and-place sequence.

## Suggested Demo Assets

- A simple tabletop simulation world.
- One manipulator with a gripper.
- Two colored blocks.
- One target bin.
- Static object poses for the first prototype.

## Non-Goals For The First Demo

- Open-ended language understanding.
- Real-time perception.
- Dynamic obstacle handling.
- Hardware execution.
- Fully autonomous recovery.
