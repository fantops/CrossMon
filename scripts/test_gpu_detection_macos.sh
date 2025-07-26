#!/bin/bash
# CrossMon - GPU Detection Test for macOS
# This script specifically tests GPU detection and enumeration

echo "========================================"
echo "CrossMon - GPU Detection Test"
echo "========================================"
echo
echo "This test will check if CrossMon can detect your GPUs correctly"
echo

# Get script directory and go to project root
cd "$(dirname "$0")/.."

# Create results directory
mkdir -p results

# Check if the executable exists
if [ ! -f "build/crossmon" ]; then
    echo "Error: CrossMon executable not found!"
    echo "Please build the project first."
    echo "Press any key to continue..."
    read dummy
    exit 1
fi

echo "Testing GPU detection..."
echo

# Start the application briefly to capture GPU detection
# Use timeout if available, otherwise run without timeout
if command -v gtimeout >/dev/null 2>&1; then
    gtimeout 6s ./build/crossmon -i 5000 > results/gpu_detection_mac.txt 2>&1 || true
else
    # Run for 6 seconds without timeout
    ./build/crossmon -i 5000 > results/gpu_detection_mac.txt 2>&1 &
    CROSSMON_PID=$!
    sleep 6
    kill $CROSSMON_PID 2>/dev/null || true
fi

echo "=== GPU Detection Results ==="
cat results/gpu_detection_mac.txt

echo
echo "Compare this with Activity Monitor > Window > GPU History"
echo "or System Information > Graphics/Displays to verify GPU detection."
echo
echo "Press any key to continue..."
read dummy
