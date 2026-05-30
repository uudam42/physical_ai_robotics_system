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
MoveIt 2 execution path is not implemented yet.
```

This parameter is present now so launch files and future configuration can stabilize before actual MoveIt 2 calls are introduced.

## Next Phase

The next phase will integrate `MoveGroupInterface` from `moveit_ros_planning_interface` behind the existing `MotionPlanner` and `GripperController` methods. The service API should remain stable while the implementation changes from mock responses to real planning and execution.
