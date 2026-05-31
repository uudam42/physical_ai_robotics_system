# Physical AI Robot Task Executor

Natural language robot instructions, validated task plans, BehaviorTree.CPP execution, and a MoveIt-ready robot action service for ROS 2 Jazzy.

## Badges

`ROS 2 Jazzy` `C++17` `Python` `BehaviorTree.CPP` `MoveIt-ready` `Physical AI`

## Overview

Physical AI Robot Task Executor is a ROS 2 robotics software engineering project that turns natural language commands into validated robot task plans, executes those plans through a C++ BehaviorTree.CPP executor, and routes robot actions through a MoveIt-ready service layer.

The project is intentionally simulation-first and safety-conscious. LLM output never controls robot actions directly. It must pass a schema validation gate, become a typed ROS 2 message, and flow through deterministic execution layers.

## Why This Project Matters

Modern robotics systems increasingly need language interfaces, but physical execution requires stricter boundaries than ordinary chat applications. This repository demonstrates a practical architecture for Physical AI:

- Python handles planner and optional LLM integration.
- ROS 2 typed messages define the planning/execution contract.
- C++ and BehaviorTree.CPP provide deterministic task execution.
- A robot action service isolates task execution from robot-specific motion planning.
- MoveIt 2 integration is prepared without forcing a robot model too early.

## System Architecture

```text
Natural language command
-> pai_llm_planner (mock/openai)
-> schema validation safety gate
-> pai_task_msgs/msg/TaskPlan
-> C++ BehaviorTree.CPP executor
-> /robot_action service
-> MoveIt-ready MotionPlanner / GripperController
-> pai_task_msgs/msg/TaskStatus
```

See [Architecture Diagram](docs/architecture_diagram.md), [Project Overview](docs/project_overview.md), and [Physical AI Architecture](docs/physical_ai_architecture.md).

## Key Features

- Natural language command intake over ROS 2.
- Mock planner and optional OpenAI planner mode.
- Schema validation for all planner outputs.
- Custom ROS 2 messages and service interfaces.
- C++ BehaviorTree.CPP executor.
- `/robot_action` service boundary for robot primitives.
- MoveIt-ready `MotionPlanner` and `GripperController` abstractions.
- Mock fallback for development without MoveIt 2 or robot hardware.
- Behavior tree XML in `bt_xml/pick_and_place.xml`.
- Placeholder MoveIt targets in `configs/moveit_targets.yaml`.

## Tech Stack

- ROS 2 Jazzy Jalisco
- Python `rclpy`
- C++17 `rclcpp`
- BehaviorTree.CPP
- Custom ROS 2 messages and services
- Optional OpenAI planner mode
- Optional MoveIt 2 build path with `BUILD_WITH_MOVEIT=ON`

## Open Source Components

- ROS 2 for node graph, topics, messages, services, and launch.
- BehaviorTree.CPP for structured task execution.
- MoveIt 2, prepared as an optional future motion planning backend.
- OpenAI Python SDK, optional and lazily imported only in OpenAI planner mode.

No fake CI badge is shown because CI has not been configured yet.

## Repository Structure

```text
.
├── bt_xml/                         # BehaviorTree.CPP XML
├── configs/                        # MoveIt target placeholders
├── docs/                           # Architecture, safety, roadmap, demo docs
├── examples/                       # Planner prompts and task-plan examples
├── ros2_ws/
│   └── src/
│       ├── pai_task_msgs/          # Task messages and robot action service
│       ├── pai_llm_planner/        # Mock/OpenAI planner node
│       ├── pai_bt_executor/        # Python prototype executor
│       ├── pai_bt_executor_cpp/    # C++ BehaviorTree.CPP executor
│       ├── pai_moveit_interface/   # Python placeholder MoveIt package
│       ├── pai_moveit_interface_cpp/ # C++ robot action service layer
│       ├── pai_robot_actions/      # Placeholder robot actions package
│       └── pai_sim_bringup/        # Launch files
└── scripts/
```

## Quick Start

Install ROS 2 Jazzy and source your environment:

```bash
source /opt/ros/jazzy/setup.bash
```

Build and source the workspace:

```bash
cd ros2_ws
colcon build
source install/setup.bash
```

## Demo Commands

Launch the full typed pipeline with the robot action service:

```bash
ros2 launch pai_sim_bringup robot_action_demo.launch.py
```

Publish a natural language command:

```bash
ros2 topic pub /user_command std_msgs/msg/String "{data: 'pick up the red cube and place it into the blue box'}"
```

Monitor execution status:

```bash
ros2 topic echo /task_status
```

Test the robot action service directly:

```bash
ros2 service call /robot_action pai_task_msgs/srv/ExecuteRobotAction "{action_name: 'move_to_home', target: 'home', parameters_json: '{}'}"
```

Inspect typed task interfaces:

```bash
ros2 interface show pai_task_msgs/msg/TaskPlan
ros2 interface show pai_task_msgs/msg/TaskStatus
ros2 interface show pai_task_msgs/srv/ExecuteRobotAction
```

## Planner Modes

Mock planner mode is the default:

```bash
ros2 run pai_llm_planner mock_llm_planner_node --ros-args -p planner_mode:=mock
```

OpenAI planner mode is optional. It reads `OPENAI_API_KEY` from the environment and still validates every output before publishing a task plan:

```bash
export OPENAI_API_KEY=your_api_key_here
ros2 run pai_llm_planner mock_llm_planner_node --ros-args -p planner_mode:=openai -p openai_model:=gpt-4o-mini
```

See [LLM Planner Safety](docs/llm_planner_safety.md), [Task Prompt Examples](examples/task_prompts.md), and [OpenAI Planner Output Examples](examples/openai_planner_output_examples.md).

## BehaviorTree.CPP Execution

The production-style executor lives in `pai_bt_executor_cpp`. It subscribes to `pai_task_msgs/msg/TaskPlan`, loads BehaviorTree.CPP XML, calls whitelisted robot action steps, and publishes `pai_task_msgs/msg/TaskStatus`.

Current tree:

```text
bt_xml/pick_and_place.xml
```

Launch the C++ BehaviorTree.CPP demo:

```bash
ros2 launch pai_sim_bringup mock_bt_cpp_demo.launch.py
```

Override the behavior tree XML path when needed:

```bash
ros2 launch pai_sim_bringup mock_bt_cpp_demo.launch.py bt_xml_path:=/absolute/path/to/bt_xml/pick_and_place.xml
```

## Robot Action Service Layer

The C++ executor calls `/robot_action` instead of controlling the robot directly. The service is implemented by `pai_moveit_interface_cpp` and currently routes actions through mock-safe `MotionPlanner` and `GripperController` classes.

This preserves a stable service contract while robot-specific MoveIt configuration evolves.

## MoveIt 2 Integration Status

MoveIt 2 support is optional behind `BUILD_WITH_MOVEIT=ON`. The default build remains mock-safe and does not require MoveIt 2.

Mock-safe build:

```bash
cd ros2_ws
colcon build
source install/setup.bash
```

MoveIt-enabled package build:

```bash
cd ros2_ws
colcon build --packages-select pai_moveit_interface_cpp --cmake-args -DBUILD_WITH_MOVEIT=ON
```

Launch the MoveIt-intent demo:

```bash
ros2 launch pai_sim_bringup moveit_robot_action_demo.launch.py
```

A real MoveIt/RViz demo still needs a robot description, SRDF, planning group, controllers, and RViz configuration. See [MoveIt Architecture](docs/moveit_architecture.md) and [MoveIt Demo Setup](docs/moveit_demo_setup.md).

## Safety Design

LLM outputs are treated as untrusted suggestions:

- Only `pick_and_place` and `unsupported` task types are accepted.
- Only whitelisted steps are accepted.
- Invalid or unsupported commands become rejected `TaskPlan` messages.
- Robot actions are executed only through the C++ BehaviorTree.CPP executor and `/robot_action` service.

This prevents hallucinated LLM actions from reaching robot execution. See [LLM Planner Safety](docs/llm_planner_safety.md).

## Testing

Run local non-ROS validation checks:

```bash
./scripts/check_all.sh
```

See [Testing](docs/testing.md) for ROS 2 build commands, package-specific builds, MoveIt-enabled build checks, and runtime test commands.

## Roadmap

- Phase 1: Mock natural-language planner and Python executor.
- Phase 2: Typed ROS 2 task messages and status reporting.
- Phase 3: C++ BehaviorTree.CPP executor.
- Phase 4: Robot action service layer.
- Phase 5: MoveIt-ready interface and demo preparation.
- Phase 6: Pluggable mock/OpenAI planner mode with validation safety gate.
- Next: Robot-specific MoveIt 2 configuration, RViz demo, and richer task plans.

See [Roadmap](docs/roadmap.md) and [Demo Plan](docs/demo_plan.md).

## Resume Highlights

- Designed a layered Physical AI architecture from language planning to robot execution.
- Built custom ROS 2 messages/services for task plans, statuses, and robot actions.
- Implemented a C++ BehaviorTree.CPP executor that consumes typed task plans.
- Added a MoveIt-ready service abstraction with mock fallback and optional compile path.
- Added a safety gate so LLM-generated plans cannot directly control robot actions.

## License

No license has been selected yet. Add a license before publishing or accepting external contributions.
