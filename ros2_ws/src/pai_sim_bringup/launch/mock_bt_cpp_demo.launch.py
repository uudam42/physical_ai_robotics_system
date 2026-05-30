import os

from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch import LaunchDescription
from launch_ros.actions import Node


def default_bt_xml_path() -> str:
    launch_dir = os.path.dirname(os.path.realpath(__file__))
    candidates = [
        os.path.join(launch_dir, "..", "..", "..", "..", "bt_xml", "pick_and_place.xml"),
        os.path.join(os.getcwd(), "bt_xml", "pick_and_place.xml"),
        os.path.join(os.getcwd(), "..", "bt_xml", "pick_and_place.xml"),
    ]

    for candidate in candidates:
        normalized_candidate = os.path.abspath(candidate)
        if os.path.exists(normalized_candidate):
            return normalized_candidate

    return os.path.abspath(candidates[0])


def generate_launch_description() -> LaunchDescription:
    return LaunchDescription(
        [
            DeclareLaunchArgument(
                "bt_xml_path",
                default_value=default_bt_xml_path(),
                description="Path to the BehaviorTree.CPP XML file.",
            ),
            Node(
                package="pai_llm_planner",
                executable="mock_llm_planner_node",
                name="mock_llm_planner_node",
                output="screen",
            ),
            Node(
                package="pai_bt_executor_cpp",
                executable="bt_executor_node",
                name="bt_executor_node",
                output="screen",
                parameters=[
                    {
                        "bt_xml_path": LaunchConfiguration("bt_xml_path"),
                    }
                ],
            ),
        ]
    )
