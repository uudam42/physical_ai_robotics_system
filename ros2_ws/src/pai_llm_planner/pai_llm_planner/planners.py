import json


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


def unsupported_plan(reason: str) -> dict:
    return {
        "task_id": "rejected_task",
        "task_type": "unsupported",
        "object_name": "",
        "source_location": "",
        "target_location": "",
        "steps": [],
        "reason": reason,
    }


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
                "steps": list(PICK_AND_PLACE_STEPS),
            }

        return unsupported_plan("Command is not supported by MockPlanner")


class OpenAIPlanner(BasePlanner):
    def __init__(self, model_name: str, api_key: str | None) -> None:
        self.model_name = model_name
        self.api_key = api_key

    def plan(self, command: str) -> dict:
        if not self.api_key:
            return unsupported_plan("OPENAI_API_KEY is not set")

        try:
            from openai import OpenAI
        except ImportError:
            return unsupported_plan("OpenAI Python package is not installed")

        try:
            client = OpenAI(api_key=self.api_key)
            response = client.chat.completions.create(
                model=self.model_name,
                messages=[
                    {
                        "role": "system",
                        "content": self._system_prompt(),
                    },
                    {
                        "role": "user",
                        "content": command,
                    },
                ],
                temperature=0,
                response_format={"type": "json_object"},
            )
            content = response.choices[0].message.content
            if not content:
                return unsupported_plan("OpenAI response was empty")
            parsed_plan = json.loads(content)
        except json.JSONDecodeError:
            return unsupported_plan("OpenAI response was not valid JSON")
        except Exception as exc:
            return unsupported_plan(f"OpenAI planner failed: {exc}")

        if not isinstance(parsed_plan, dict):
            return unsupported_plan("OpenAI response JSON was not an object")
        return parsed_plan

    def _system_prompt(self) -> str:
        return """
Convert natural language robot commands into a JSON task plan.

Supported task_type values:
- pick_and_place
- unsupported

Supported steps only:
- check_object
- move_to_pre_grasp
- open_gripper
- move_to_grasp
- close_gripper
- move_to_target
- return_home

Do not invent unsupported robot actions.
If the command is not supported, output task_type="unsupported" with empty steps.
Output JSON only.

Expected JSON fields:
- task_id
- task_type
- object_name
- source_location
- target_location
- steps
""".strip()
