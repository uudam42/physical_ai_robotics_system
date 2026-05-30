from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description() -> LaunchDescription:
    return LaunchDescription(
        [
            Node(
                package="pai_llm_planner",
                executable="mock_llm_planner_node",
                name="mock_llm_planner_node",
                output="screen",
            ),
            Node(
                package="pai_bt_executor",
                executable="mock_bt_executor_node",
                name="mock_bt_executor_node",
                output="screen",
            ),
        ]
    )
