PICK_AND_PLACE_STEPS = [
    "check_object",
    "move_to_pre_grasp",
    "open_gripper",
    "move_to_grasp",
    "close_gripper",
    "move_to_target",
    "open_gripper",
    "return_home",
]


class BasePlanner:
    def plan(self, command: str) -> dict:
        raise NotImplementedError


class MockPlanner(BasePlanner):
    def plan(self, command: str) -> dict:
        normalized_command = command.lower()
        required_words = ("pick", "cube", "box")

        if all(word in normalized_command for word in required_words):
            return {
                "task_id": "demo_task_001",
                "task_type": "pick_and_place",
                "object_name": "red_cube",
                "source_location": "table",
                "target_location": "blue_box",
                "steps": PICK_AND_PLACE_STEPS,
            }

        return {
            "task_id": "rejected_task",
            "task_type": "unsupported",
            "object_name": "",
            "source_location": "",
            "target_location": "",
            "steps": [],
        }
