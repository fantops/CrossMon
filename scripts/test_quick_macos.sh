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

# Create results directory if it doesn't exist
mkdir -p results

# Create console output log file with timestamp
CONSOLE_LOG="results/crossmon_console_quick_$(date +%Y%m%d_%H%M%S).log"
cat > "$CONSOLE_LOG" << EOF

CrossMon Quick Test Console Output
Generated: $(date)
========================================

EOF

echo "Console output will be saved to: $CONSOLE_LOG"

# Start CrossMon with console output capture
echo "Starting CrossMon with console output capture..."
./build/crossmon -i 2000 >> "$CONSOLE_LOG" 2>&1 &
CROSSMON_PID=$!

# Wait a moment for process to start
sleep 2

# Check if process is still running
if kill -0 $CROSSMON_PID 2>/dev/null; then
    echo "CrossMon is running (PID: $CROSSMON_PID), monitoring for 13 more seconds..."
    sleep 13
    
    # Try graceful termination first
    echo "Stopping CrossMon..."
    if kill -TERM $CROSSMON_PID 2>/dev/null; then
        # Wait up to 3 seconds for graceful shutdown
        for i in 1 2 3; do
            if ! kill -0 $CROSSMON_PID 2>/dev/null; then
                break
            fi
            sleep 1
        done
        
        # Force kill if still running
        if kill -0 $CROSSMON_PID 2>/dev/null; then
            echo "Forcing termination..."
            kill -KILL $CROSSMON_PID 2>/dev/null || true
        fi
    fi
    echo "Test completed!"
    echo "Console output saved to: $CONSOLE_LOG"
else
    echo "Warning: CrossMon failed to start or exited early"
fi

echo
echo "=== Console Output Preview ==="
if [ -f "$CONSOLE_LOG" ]; then
    tail -20 "$CONSOLE_LOG"
else
    echo "No console output captured"
fi

echo
echo "Test completed! Full console output saved to: $CONSOLE_LOG"
echo "Press any key to continue..."
read dummy
