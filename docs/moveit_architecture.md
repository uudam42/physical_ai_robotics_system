# MoveIt-Ready Architecture

Phase 5.1 introduces a MoveIt-ready abstraction layer inside `pai_moveit_interface_cpp` while keeping all robot actions mocked by default.

## Purpose

The `/robot_action` service remains the stable contract between the BehaviorTree.CPP executor and the future robot motion layer. Internally, the service now routes requests through focused components:

- `RobotActionServer`: owns the ROS 2 service and request routing.
- `MotionPlanner`: owns arm motion commands such as pre-grasp, grasp, target moves, and home moves.
- `GripperController`: owns gripper open and close commands.

## Current Mode

`MotionPlanner` and `GripperController` are mock implementations today. With the default parameter:

```text
use_moveit: false
```

all supported robot actions return mock success messages. This keeps the full task pipeline runnable without requiring robot models, planning groups, controllers, or MoveIt 2 configuration.

## MoveIt Placeholder Mode

The `robot_action_server` declares a `use_moveit` parameter. When `use_moveit` is set to `true`, the motion and gripper components intentionally return:

```text
MoveIt 2 support is disabled at build time. Rebuild with BUILD_WITH_MOVEIT=ON.
```

This parameter is present now so launch files and future configuration can stabilize before actual MoveIt 2 calls are introduced.

## Phase 5.2 Optional MoveIt Build

MoveIt 2 support is now guarded by both a build-time switch and a runtime parameter.

`BUILD_WITH_MOVEIT=OFF` is the default mock-safe build. In this mode, `pai_moveit_interface_cpp` does not include MoveIt 2 headers and does not require `moveit_ros_planning_interface`.

`BUILD_WITH_MOVEIT=ON` enables the MoveIt 2 compile path and defines `PAI_BUILD_WITH_MOVEIT`. Even in that build, the `use_moveit` runtime parameter controls whether the server tries the MoveIt path or stays in mock fallback mode.

Both switches are intentional:

- The build-time switch protects environments that do not have MoveIt 2 installed.
- The runtime switch allows mock fallback even in MoveIt-capable builds.

Mock-only build:

```bash
cd ros2_ws
colcon build --packages-select pai_moveit_interface_cpp
```

MoveIt-enabled build:

```bash
cd ros2_ws
colcon build --packages-select pai_moveit_interface_cpp --cmake-args -DBUILD_WITH_MOVEIT=ON
```

Run with mock mode:

```bash
ros2 run pai_moveit_interface_cpp robot_action_server --ros-args -p use_moveit:=false
```

Run with MoveIt mode:

```bash
ros2 run pai_moveit_interface_cpp robot_action_server --ros-args -p use_moveit:=true -p planning_group:=arm
```

When MoveIt support is compiled but a robot model and planning group are not configured, the current placeholder path returns:

```text
MoveIt 2 path is compiled but robot model / planning group is not configured yet.
```

## Next Phase

The next phase will integrate `MoveGroupInterface` from `moveit_ros_planning_interface` behind the existing `MotionPlanner` and `GripperController` methods. The service API should remain stable while the implementation changes from mock responses to real planning and execution.
