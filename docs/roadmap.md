# Roadmap

## Phase 0: Repository Scaffold

- Create ROS 2 workspace structure.
- Add placeholder packages.
- Document architecture and demo direction.
- Add placeholder configuration, behavior tree, example, and script directories.

## Phase 1: Typed Task Planning Contracts

- Define task request, task plan, task step, and execution status messages.
- Add schema validation for planner output.
- Add example task plans in `examples/`.
- Add unit tests for task plan validation.

## Phase 2: LLM Planner Prototype

- Implement a planner node that accepts natural language task requests.
- Generate structured task plans from constrained prompts.
- Validate generated plans before execution.
- Add deterministic fixture-based tests.

## Phase 3: Behavior Tree Execution

- Convert task plans into BehaviorTree.CPP XML.
- Add a behavior tree runner node.
- Implement placeholder action nodes.
- Add logging and execution status reporting.

## Phase 4: MoveIt 2 Simulation Integration

- Connect robot actions to MoveIt 2 planning APIs.
- Add simulated robot bringup.
- Support basic pick-and-place workflows.
- Add collision object and planning scene examples.

## Phase 5: Robust Manipulation Demos

- Add recovery behavior for planning and execution failures.
- Add multi-step task demos.
- Add execution replay and diagnostics.
- Improve simulation realism.

## Phase 6: Hardware Readiness

- Separate simulator-specific logic from hardware interfaces.
- Add safety checks and operator confirmation points.
- Integrate real robot drivers.
- Document deployment and calibration workflows.
