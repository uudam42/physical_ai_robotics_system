# Resume Bullets

## Strong Resume Bullets

- Built a ROS 2 Jazzy Physical AI task execution stack that translates natural language commands into validated typed task plans and executes them through a C++ BehaviorTree.CPP pipeline.
- Designed a safe LLM planning layer in Python with mock and optional OpenAI planner modes, schema validation, and restricted action steps before publishing ROS 2 `TaskPlan` messages.
- Implemented a MoveIt-ready robot action service architecture with C++ `MotionPlanner` and `GripperController` abstractions, mock fallback, and optional MoveIt 2 compile path.

## Stronger Technical Resume Bullets

- Developed custom ROS 2 messages and services for task planning, execution status, and robot action requests, enabling a typed contract between Python planning nodes and C++ robotics execution nodes.
- Integrated BehaviorTree.CPP into a C++ ROS 2 executor that consumes validated task plans, loads XML behavior trees, calls a `/robot_action` service, and publishes typed task status updates.
- Architected an LLM-safe robotics pipeline where hallucinated or unsupported actions are rejected by schema validation before reaching BehaviorTree.CPP execution or robot action services.

## Short Project Description

Physical AI Robot Task Executor is a ROS 2 Jazzy robotics software project that connects natural language planning, safe task-plan validation, C++ BehaviorTree.CPP execution, and a MoveIt-ready robot action service layer. The current system runs with mock robot actions and is prepared for future robot-specific MoveIt 2 integration.

## GitHub Project Tagline

Safe natural-language robot task planning with ROS 2, BehaviorTree.CPP, typed messages, and a MoveIt-ready execution architecture.
