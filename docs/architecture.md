# Architecture

This document describes the intended architecture for the Physical AI Robot Task Executor. The current repository contains placeholders only; the sections below define the direction for future implementation.

## System Overview

The system is designed as a simulation-first ROS 2 Jazzy stack:

```text
Natural language instruction
        |
        v
pai_llm_planner
        |
        v
pai_task_msgs task plan
        |
        v
pai_bt_executor
        |
        v
BehaviorTree.CPP XML
        |
        v
pai_robot_actions
        |
        v
pai_moveit_interface
        |
        v
MoveIt 2 + simulator
```

## Package Responsibilities

### pai_task_msgs

Defines shared interfaces for task planning and execution. Future messages may represent:

- User task requests
- Structured task plans
- Task steps
- Execution status
- Failure reasons

### pai_llm_planner

Receives a natural language instruction and produces a structured robot task plan. Future work should separate prompt construction, model calls, validation, and recovery behavior.

### pai_bt_executor

Converts structured task plans into BehaviorTree.CPP trees and manages execution lifecycle. Future work should include tree validation, execution tracing, cancellation, retry policies, and failure reporting.

### pai_robot_actions

Provides reusable robot action primitives such as pick, place, open, close, inspect, move-to-pose, and wait-for-condition. These actions should expose stable ROS 2 action or service interfaces.

### pai_moveit_interface

Wraps MoveIt 2 planning scene updates, motion planning requests, gripper control integration, and execution monitoring behind a project-specific interface.

### pai_sim_bringup

Owns simulation launch files, robot model selection, world setup, MoveIt configuration references, and demo startup orchestration.

## Data Flow

1. A task instruction enters the system.
2. The planner converts the instruction into a structured plan.
3. The behavior tree executor validates and converts the plan into a tree.
4. Behavior tree action nodes call robot action primitives.
5. Robot actions call MoveIt 2 and simulator-facing interfaces.
6. Execution status flows back to the user-facing layer.

## Design Principles

- Keep natural language planning separate from robot execution.
- Treat LLM output as untrusted until validated.
- Prefer typed ROS 2 messages for cross-package contracts.
- Keep manipulation primitives small and reusable.
- Test in simulation before hardware integration.
- Design every execution step for observability and recovery.

## Future Integration Points

- LLM provider abstraction
- Task plan schema validation
- BehaviorTree.CPP plugin nodes
- MoveIt Task Constructor
- Perception and scene understanding
- Robot hardware drivers
- Telemetry and execution replay
