# Future Work

## Real MoveIt 2 Robot Model Integration

Add a selected robot arm model with URDF or xacro, SRDF, kinematics configuration, planning groups, and controller mappings.

## RViz Demo With Robot Arm

Add a launchable RViz demo that shows the robot model, planning scene, target objects, and planned trajectories.

## Gazebo / Isaac Sim Simulation

Connect the task pipeline to a simulator such as Gazebo or Isaac Sim so manipulation behavior can be tested before hardware deployment.

## Perception Module For Object Detection

Add perception nodes that detect objects, estimate poses, and update the planning scene. This would replace symbolic object names with actual scene information.

## Real Gripper / Controller Integration

Connect `GripperController` and `MotionPlanner` to real robot controllers, gripper drivers, and trajectory execution feedback.

## Failure Recovery And Retry Logic

Extend BehaviorTree.CPP XML with retries, fallbacks, precondition checks, timeout handling, and operator intervention points.

## CI With ROS 2 Docker

Add GitHub Actions or another CI system using a ROS 2 Jazzy Docker image to run package builds, Python checks, XML validation, and interface generation checks.

## Fast DDS / QoS Benchmarking

Benchmark ROS 2 middleware behavior, QoS profiles, service latency, and task-status update timing under different execution loads.

## Autoware-Inspired Autonomous Systems Direction

Extend the architecture toward autonomous systems patterns inspired by Autoware: modular perception, planning, behavior execution, control interfaces, simulation-first validation, and safety-oriented typed boundaries.
