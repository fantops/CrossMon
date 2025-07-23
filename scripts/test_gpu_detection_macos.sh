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

# Check if the executable exists
if [ ! -f "build/crossmon" ]; then
    echo "Error: CrossMon executable not found!"
    echo "Please build the project first."
    read -p "Press any key to continue..."
    exit 1
fi

echo "Testing GPU detection..."
echo

# Start the application briefly to capture GPU detection
timeout 6s ./build/crossmon -i 5000 > gpu_detection_mac.txt 2>&1 || true

echo "=== GPU Detection Results ==="
cat gpu_detection_mac.txt

echo
echo "Compare this with Activity Monitor > Window > GPU History"
echo "or System Information > Graphics/Displays to verify GPU detection."
echo
read -p "Press any key to continue..."
