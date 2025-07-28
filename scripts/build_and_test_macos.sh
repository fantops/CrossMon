#!/bin/bash
# CrossMon - Unified Build, Test, and Quick Monitor Script for macOS
# Usage: ./build_and_test_macos.sh [debug|release]

set -e
BUILD_TYPE="release"
if [ $# -ge 1 ]; then
    arg=$(echo "$1" | tr '[:upper:]' '[:lower:]')
    if [[ "$arg" == "debug" ]]; then
        BUILD_TYPE="debug"
    fi
fi
LOG_FILE="results/build_and_test_${BUILD_TYPE}.log"

mkdir -p results

# Step 1: Build
BUILD_DIR="build"
if [ "$BUILD_TYPE" = "debug" ]; then
    BUILD_DIR="build_debug"
fi
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
echo "Configuring with CMake..." | tee -a "../$LOG_FILE"
# Capitalize first letter for CMake (bash/zsh compatible)
BUILD_TYPE_CAP=$(echo "$BUILD_TYPE" | sed 's/^./\U&/')
cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE_CAP >> "../$LOG_FILE" 2>&1
if [ $? -ne 0 ]; then
    echo "Error: CMake configuration failed!" | tee -a "../$LOG_FILE"
    echo "Press any key to continue..."
    read dummy
    exit 1
fi
echo "Building project..." | tee -a "../$LOG_FILE"
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4) >> "../$LOG_FILE" 2>&1
if [ $? -ne 0 ]; then
    echo "Error: Build failed!" | tee -a "../$LOG_FILE"
    echo "Press any key to continue..."
    read dummy
    exit 1
fi
cd ..
echo "Build completed successfully!" | tee -a "$LOG_FILE"

# Step 2: Run unit tests
cd "$BUILD_DIR"
echo "Running all unit tests (including GPU tests)..." | tee -a "../$LOG_FILE"
ctest --output-on-failure >> "../$LOG_FILE" 2>&1
if [ $? -ne 0 ]; then
    echo "Warning: Some unit tests failed. Check log for details." | tee -a "../$LOG_FILE"
else
    echo "All unit tests passed!" | tee -a "../$LOG_FILE"
fi
cd ..

echo "Step 3: Quick system monitor test..." | tee -a "$LOG_FILE"
if [ -f "$BUILD_DIR/crossmon" ]; then
    echo "Running CrossMon for 15 seconds with 2s intervals..." | tee -a "$LOG_FILE"
    
    # Create console output log file
    CONSOLE_LOG="results/crossmon_console_${BUILD_TYPE}.log"
    mkdir -p results
    cat > "$CONSOLE_LOG" << EOF

CrossMon Console Output Log - ${BUILD_TYPE} Build
Generated: $(date)
========================================

EOF
    
    # Start CrossMon with console output capture
    echo "Starting CrossMon with console output capture..." | tee -a "$LOG_FILE"
    ./$BUILD_DIR/crossmon -i 2000 >> "$CONSOLE_LOG" 2>&1 &
    CROSSMON_PID=$!
    
    # Wait a moment for process to start
    sleep 2
    
    # Check if process is still running
    if kill -0 $CROSSMON_PID 2>/dev/null; then
        echo "CrossMon is running (PID: $CROSSMON_PID), monitoring for 13 more seconds..." | tee -a "$LOG_FILE"
        sleep 13
        
        # Try graceful termination first
        echo "Stopping CrossMon..." | tee -a "$LOG_FILE"
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
                echo "Forcing termination..." | tee -a "$LOG_FILE"
                kill -KILL $CROSSMON_PID 2>/dev/null || true
            fi
        fi
    else
        echo "Warning: CrossMon failed to start or exited early" | tee -a "$LOG_FILE"
    fi
    
    echo "Quick test completed!" | tee -a "$LOG_FILE"
    echo "Console output saved to: $CONSOLE_LOG" | tee -a "$LOG_FILE"
else
    echo "Error: crossmon not found in $BUILD_DIR!" | tee -a "$LOG_FILE"
fi

echo "All tests completed!" | tee -a "$LOG_FILE"
echo "Results saved to: $LOG_FILE"
echo "Press any key to continue..."
read dummy
