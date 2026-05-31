#!/bin/sh

set -u

xml_file="bt_xml/pick_and_place.xml"

echo "==> Checking BehaviorTree.CPP XML"

if ! command -v xmllint >/dev/null 2>&1; then
  echo "WARNING: xmllint is not available; skipping XML validation"
  exit 0
fi

if xmllint --noout "$xml_file"; then
  echo "OK: $xml_file is valid XML"
else
  echo "ERROR: $xml_file failed XML validation" >&2
  exit 1
fi
