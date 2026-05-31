#!/bin/sh

set -u

scripts/check_python.sh
scripts/check_docs.sh
scripts/check_bt_xml.sh

echo "OK: All local validation checks passed"
