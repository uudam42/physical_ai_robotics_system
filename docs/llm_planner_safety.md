# LLM Planner Safety

The LLM planner is used only for high-level task planning. It does not directly call robot actions, motion planning APIs, gripper commands, or hardware drivers.

The planner pipeline is intentionally constrained:

- Natural language is converted into a task-plan dictionary.
- Every planner output must pass schema validation.
- Only allowed task types are accepted.
- Only whitelisted action steps are accepted.
- Unsupported or invalid commands become rejected `TaskPlan` messages.
- The C++ BehaviorTree.CPP executor is responsible for structured execution.
- The `/robot_action` service is responsible for robot action requests.

This design reduces risk from hallucinated actions. If an LLM invents a step such as `throw_object`, `cut_object`, or `drive_to_kitchen`, validation rejects the plan before it becomes a typed ROS 2 message for execution.

## Why Not Let The LLM Control The Robot Directly?

Direct control is unsafe for physical systems. LLMs can hallucinate actions, omit safety-critical steps, misunderstand the robot state, or generate commands that do not exist in the robot stack.

Physical systems need deterministic execution layers. BehaviorTree.CPP provides structured execution, explicit control flow, and predictable failure handling. ROS 2 typed messages provide clear interfaces between planning, execution, and robot action services.

The safer pattern is:

```text
LLM suggestion -> schema validation -> typed task plan -> deterministic executor -> robot action service
```

The LLM may suggest what task should happen, but the robotics system decides whether the plan is valid and how execution is performed.
