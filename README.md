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

At this stage, the packages are placeholders. The build verifies package structure only.

## Development Status

This repository is at the initial scaffold stage. See:

- [Architecture](docs/architecture.md)
- [Roadmap](docs/roadmap.md)
- [Demo Plan](docs/demo_plan.md)

## License

No license has been selected yet. Add a license before publishing or accepting external contributions.
