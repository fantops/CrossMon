#!/bin/bash
# CrossMon - NPU Detection Test for macOS

echo "========================================"
echo "CrossMon - NPU Detection Test (macOS)"
echo "========================================"
echo
mkdir -p results
RESULT_FILE=results/npu_detection_mac.txt
echo "NPU monitoring is not supported on macOS." | tee $RESULT_FILE 