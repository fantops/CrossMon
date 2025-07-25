#!/bin/bash
# CrossMon - Quick System Monitor Test for macOS
# This script runs CrossMon for 15 seconds to test basic functionality

echo "========================================"
echo "CrossMon - Quick System Monitor Test"
echo "========================================"
echo
echo "This will monitor your system for 15 seconds"
echo "Press Ctrl+C to stop early if needed"
echo

# Get script directory and go to project root
cd "$(dirname "$0")/.."

# Check if the executable exists
if [ ! -f "build/crossmon" ]; then
    echo "Error: CrossMon executable not found!"
    echo "Please build the project first using:"
    echo "  mkdir -p build && cd build"
    echo "  cmake .. && make"
    echo
    echo "Press any key to continue..."
    read dummy
    exit 1
fi

echo "Starting CrossMon with 2-second intervals..."
echo

# Run in background for about 15 seconds
gtimeout 15s ./build/crossmon -i 2000 > test_results_mac.txt 2>&1 || true

echo
echo "=== Test Results ==="
cat test_results_mac.txt

echo
echo "Test completed! Check test_results_mac.txt for full output."
echo "Press any key to continue..."
read dummy
