#!/bin/sh

set -u

echo "==> Checking Python planner files"
if python3 -m compileall ros2_ws/src/pai_llm_planner; then
  echo "OK: Python planner files compile"
else
  echo "ERROR: Python planner compile check failed" >&2
  exit 1
fi

echo "==> Checking task plan schema JSON"
if python3 -m json.tool examples/task_plan_schema.json >/dev/null; then
  echo "OK: examples/task_plan_schema.json is valid JSON"
else
  echo "ERROR: examples/task_plan_schema.json is invalid JSON" >&2
  exit 1
fi

echo "OK: Python and JSON checks passed"
