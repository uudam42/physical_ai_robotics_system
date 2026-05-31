# Testing

This project has lightweight local checks plus ROS 2 build and runtime checks.

## Local Non-ROS Checks

Run all local validation scripts:

```bash
./scripts/check_all.sh
```

This runs:

- Python planner compile check
- Task plan schema JSON validation
- Required documentation file checks
- BehaviorTree.CPP XML validation when `xmllint` is available

## ROS 2 Build Check

```bash
cd ros2_ws
colcon build
```

## Package-Specific Builds

```bash
colcon build --packages-select pai_task_msgs
colcon build --packages-select pai_llm_planner
colcon build --packages-select pai_bt_executor_cpp
colcon build --packages-select pai_moveit_interface_cpp
```

## MoveIt-Enabled Build

Use this only in an environment where MoveIt 2 is installed:

```bash
colcon build --packages-select pai_moveit_interface_cpp --cmake-args -DBUILD_WITH_MOVEIT=ON
```

## Runtime Tests

Launch the service-backed task pipeline:

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

Call the robot action service directly:

```bash
ros2 service call /robot_action pai_task_msgs/srv/ExecuteRobotAction "{action_name: 'move_to_home', target: 'home', parameters_json: '{}'}"
```
