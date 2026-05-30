# MoveIt Demo Setup

Phase 5.3 prepares the project for a MoveIt 2 and RViz demo without selecting a specific robot model yet.

## Required Environment

- Ubuntu 24.04
- ROS 2 Jazzy
- MoveIt 2
- `colcon`

## Mock-Only Build

The default build remains safe for environments without MoveIt 2:

```bash
cd ros2_ws
colcon build
source install/setup.bash
```

## MoveIt-Enabled Package Build

Enable the optional MoveIt compile path only when MoveIt 2 is installed:

```bash
cd ros2_ws
colcon build --packages-select pai_moveit_interface_cpp --cmake-args -DBUILD_WITH_MOVEIT=ON
source install/setup.bash
```

## Current Robot Action Demo

Launch the current service-backed pipeline:

```bash
ros2 launch pai_sim_bringup robot_action_demo.launch.py
```

Run the robot action server directly with the MoveIt runtime path enabled:

```bash
ros2 run pai_moveit_interface_cpp robot_action_server --ros-args -p use_moveit:=true -p planning_group:=arm
```

## Robot-Specific Requirements

A real MoveIt demo still requires robot-specific assets:

- Robot description, usually URDF or xacro
- Semantic robot description, usually SRDF
- A valid planning group
- Joint trajectory controllers or fake controllers
- MoveIt controller configuration
- Planning scene and kinematics configuration
- RViz configuration

This repository intentionally separates the `/robot_action` service contract from robot-specific MoveIt configuration. That keeps the planner, BehaviorTree.CPP executor, and action service stable while a robot model is selected later.

## Placeholder Targets

Example target names and placeholder poses live in:

```text
configs/moveit_targets.yaml
```

These values are not tied to a real robot yet. Adapt the planning group, named targets, frames, and poses to the selected robot model before attempting real motion planning.
