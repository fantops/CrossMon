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

# Create results directory
mkdir -p results

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
# Use timeout if available, otherwise run without timeout
if command -v gtimeout >/dev/null 2>&1; then
    gtimeout 15s ./build/crossmon -i 2000 > results/test_results_mac.txt 2>&1 || true
else
    # Run for 15 seconds without timeout
    ./build/crossmon -i 2000 > results/test_results_mac.txt 2>&1 &
    CROSSMON_PID=$!
    sleep 15
    kill $CROSSMON_PID 2>/dev/null || true
fi

echo
echo "=== Test Results ==="
cat results/test_results_mac.txt

echo
echo "Test completed! Check results/test_results_mac.txt for full output."
echo "Press any key to continue..."
read dummy
