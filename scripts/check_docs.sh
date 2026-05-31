#!/bin/sh

set -u

missing=0

check_file() {
  if [ -f "$1" ]; then
    echo "OK: $1"
  else
    echo "MISSING: $1" >&2
    missing=1
  fi
}

echo "==> Checking required documentation files"
check_file README.md
check_file docs/architecture_diagram.md
check_file docs/project_overview.md
check_file docs/physical_ai_architecture.md
check_file docs/llm_planner_safety.md
check_file docs/moveit_architecture.md
check_file docs/moveit_demo_setup.md

if [ "$missing" -ne 0 ]; then
  echo "ERROR: One or more documentation files are missing" >&2
  exit 1
fi

echo "OK: Documentation files are present"
