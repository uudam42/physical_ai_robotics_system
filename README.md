# Physical AI Robot Task Executor

Physical AI Robot Task Executor is an early-stage ROS 2 Jazzy robotics project for translating natural language instructions into robot task plans, converting those plans into BehaviorTree.CPP behavior trees, and executing robot manipulation actions through ROS 2 and MoveIt 2 in simulation.

The current repository focuses on clean structure, documentation, and future extensibility. It does not implement production task planning, behavior tree execution, or motion planning logic yet.

## Goals

- Accept natural language task instructions from a user or higher-level application.
- Convert instructions into structured robot task plans.
- Represent executable task logic as BehaviorTree.CPP XML.
- Execute manipulation actions through ROS 2 nodes and MoveIt 2 interfaces.
- Support simulation-first development before hardware deployment.

## Repository Layout

```text
.
├── README.md
├── docs/
│   ├── architecture.md
│   ├── demo_plan.md
│   └── roadmap.md
├── ros2_ws/
│   └── src/
│       ├── pai_task_msgs/
│       ├── pai_llm_planner/
│       ├── pai_bt_executor/
│       ├── pai_robot_actions/
│       ├── pai_moveit_interface/
│       └── pai_sim_bringup/
├── bt_xml/
├── configs/
├── examples/
├── scripts/
└── .gitignore
```

## Placeholder ROS 2 Packages

- `pai_task_msgs`: shared task and behavior planning message definitions.
- `pai_llm_planner`: future natural-language-to-task-plan planning node.
- `pai_bt_executor`: future BehaviorTree.CPP execution layer.
- `pai_robot_actions`: future robot action servers and manipulation primitives.
- `pai_moveit_interface`: future MoveIt 2 planning and execution adapter.
- `pai_sim_bringup`: future simulation launch and environment setup package.

## Expected Platform

- Ubuntu 24.04
- ROS 2 Jazzy Jalisco
- MoveIt 2 for Jazzy
- BehaviorTree.CPP
- Gazebo or another ROS 2-compatible simulator

## Getting Started

Install ROS 2 Jazzy and source your environment:

```bash
source /opt/ros/jazzy/setup.bash
```

Build the workspace:

```bash
cd ros2_ws
colcon build
```

Source the local workspace:

```bash
source install/setup.bash
```

## Phase 1 Quick Start

Phase 1 introduced a minimal mock communication pipeline:

```text
natural language command -> mock LLM planner -> task plan -> mock behavior-tree executor
```

Build the workspace:

```bash
cd ros2_ws
colcon build
```

Source the local workspace:

```bash
source install/setup.bash
```

Launch the mock demo:

```bash
ros2 launch pai_sim_bringup mock_demo.launch.py
```

In another terminal, source the workspace and publish a test command:

```bash
source install/setup.bash
ros2 topic pub /user_command std_msgs/msg/String "{data: 'pick up the red cube and place it into the blue box'}"
```

Expected behavior:

- The mock planner receives the natural language command.
- It publishes a task plan to `/task_plan`.
- The mock executor receives the plan and logs each simulated robot step.

Phase 1 used `std_msgs/String` and JSON for rapid prototyping. Phase 2 keeps this command flow but upgrades the internal task plan topic to typed ROS 2 messages.

## Phase 2 Typed Task Pipeline

Phase 2 replaces JSON string task plans on `/task_plan` with custom ROS 2 messages from `pai_task_msgs`. The user command still enters the system as `std_msgs/String` on `/user_command`, but the planner now publishes `pai_task_msgs/msg/TaskPlan`, and the executor publishes execution updates as `pai_task_msgs/msg/TaskStatus` on `/task_status`.

Inspect the typed task interfaces:

```bash
ros2 interface show pai_task_msgs/msg/TaskPlan
ros2 interface show pai_task_msgs/msg/TaskStatus
```

Monitor task execution status:

```bash
ros2 topic echo /task_status
```

The Phase 1 command flow is preserved: launch `mock_demo.launch.py`, publish a natural language command to `/user_command`, and watch the mock planner and executor communicate through the typed task pipeline.

## Phase 3 C++ BehaviorTree Executor

Phase 3 adds `pai_bt_executor_cpp`, a C++ ROS 2 package that uses BehaviorTree.CPP for production-style task orchestration. The Python executor remains available as a rapid prototype path, while the C++ BehaviorTree.CPP executor is the intended robotics execution path for future manipulation integration.

The current behavior tree is defined in:

```text
bt_xml/pick_and_place.xml
```

Build and source the workspace:

```bash
cd ros2_ws
colcon build
source install/setup.bash
```

Launch the C++ BehaviorTree.CPP demo:

```bash
ros2 launch pai_sim_bringup mock_bt_cpp_demo.launch.py
```

Publish a natural language command:

```bash
ros2 topic pub /user_command std_msgs/msg/String "{data: 'pick up the red cube and place it into the blue box'}"
```

Monitor task status:

```bash
ros2 topic echo /task_status
```

The launch file passes `bt_xml_path` to the C++ executor. If the repository-level XML path is not valid after installation or relocation, override it manually:

```bash
ros2 launch pai_sim_bringup mock_bt_cpp_demo.launch.py bt_xml_path:=/absolute/path/to/bt_xml/pick_and_place.xml
```

## Phase 4 Robot Action Service Layer

Phase 4 introduces a robot action service interface between the C++ BehaviorTree.CPP executor and the future motion planning layer. BehaviorTree.CPP nodes now call `/robot_action`, and `pai_moveit_interface_cpp` currently serves those requests with mocked robot actions.

This prepares the system for MoveIt 2 integration in the next phase while keeping the service contract stable first. Actual MoveIt 2 `MoveGroupInterface` integration is intentionally deferred.

Current architecture:

```text
Natural language command
-> typed TaskPlan
-> C++ BehaviorTree.CPP executor
-> /robot_action service
-> mocked MoveIt interface
-> typed TaskStatus
```

Build and source the workspace:

```bash
cd ros2_ws
colcon build
source install/setup.bash
```

Launch the robot action service demo:

```bash
ros2 launch pai_sim_bringup robot_action_demo.launch.py
```

Publish a natural language command:

```bash
ros2 topic pub /user_command std_msgs/msg/String "{data: 'pick up the red cube and place it into the blue box'}"
```

Monitor task status:

```bash
ros2 topic echo /task_status
```

Optionally test the robot action service directly:

```bash
ros2 service call /robot_action pai_task_msgs/srv/ExecuteRobotAction "{action_name: 'move_to_home', target: 'home', parameters_json: '{}'}"
```

## Phase 5 MoveIt-Ready Interface

MoveIt 2 support is optional behind `BUILD_WITH_MOVEIT=ON`. The default mode remains the mock fallback, so the project can still build and run in environments without MoveIt 2 installed. A full robot model and RViz demo are planned for the next phase.

## Development Status

This repository is at the initial scaffold stage. See:

- [Architecture](docs/architecture.md)
- [Roadmap](docs/roadmap.md)
- [Demo Plan](docs/demo_plan.md)

## License

No license has been selected yet. Add a license before publishing or accepting external contributions.
