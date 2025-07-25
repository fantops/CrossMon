#!/bin/bash
# CrossMon - Application Monitoring Test for macOS
# This script tests monitoring while a specific application runs

echo "========================================"
echo "CrossMon - Application Monitoring Test"
echo "========================================"
echo
echo "This test will monitor system usage while TextEdit is running"
echo

# Get script directory and go to project root
cd "$(dirname "$0")/.."

# Check if the executable exists
if [ ! -f "build/crossmon" ]; then
    echo "Error: CrossMon executable not found!"
    echo "Please build the project first."
    echo "Press any key to continue..."
    read dummy
    exit 1
fi

echo "Starting TextEdit and monitoring system usage..."
echo "Close TextEdit to stop monitoring or press Ctrl+C"
echo

# Create output directory
mkdir -p results

echo "Monitoring will start in 2 seconds..."
sleep 2

# Run CrossMon to monitor while TextEdit is running
./build/crossmon -i 1500 -o results/textedit_monitoring.txt TextEdit

echo
echo "=== Monitoring Results ==="
if [ -f "results/textedit_monitoring.txt" ]; then
    cat results/textedit_monitoring.txt
else
    echo "No results file found. Check for errors."
fi

echo
echo "Test completed!"
echo "Press any key to continue..."
read dummy
