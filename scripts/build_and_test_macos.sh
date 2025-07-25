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
cmake .. -DCMAKE_BUILD_TYPE=${BUILD_TYPE^} >> "../$LOG_FILE" 2>&1
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
    gtimeout 15s ./$BUILD_DIR/crossmon -i 2000 >> "$LOG_FILE" 2>&1 || true
    echo "Quick test completed!" | tee -a "$LOG_FILE"
else
    echo "Error: crossmon not found in $BUILD_DIR!" | tee -a "$LOG_FILE"
fi

echo "All tests completed!" | tee -a "$LOG_FILE"
echo "Results saved to: $LOG_FILE"
echo "Press any key to continue..."
read dummy
