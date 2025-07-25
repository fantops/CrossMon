#!/bin/bash
# CrossMon - Build and Test Script for macOS
# This script builds CrossMon and runs basic tests

echo "========================================"
echo "CrossMon - Build and Test Script"
echo "========================================"
echo

# Get script directory and go to project root
cd "$(dirname "$0")/.."

# Create results directory if it doesn't exist
mkdir -p results

LOG_FILE="results/build_and_test.log"

echo "Logging output to: $LOG_FILE"
echo

echo "Step 1: Building CrossMon..." | tee -a "$LOG_FILE"
echo

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Configure with CMake
echo "Configuring with CMake..." | tee -a "../$LOG_FILE"
cmake .. -DCMAKE_BUILD_TYPE=Release >> "../$LOG_FILE" 2>&1

if [ $? -ne 0 ]; then
    echo "Error: CMake configuration failed!" | tee -a "../$LOG_FILE"
    read -p "Press any key to continue..."
    exit 1
fi

# Build the project
echo "Building project..." | tee -a "../$LOG_FILE"
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4) >> "../$LOG_FILE" 2>&1

if [ $? -ne 0 ]; then
    echo "Error: Build failed!" | tee -a "../$LOG_FILE"
    read -p "Press any key to continue..."
    exit 1
fi

cd ..

echo
echo "Build completed successfully!" | tee -a "$LOG_FILE"
echo

echo "Step 2: Running unit tests..." | tee -a "$LOG_FILE"
echo
cd build
echo "Running all unit tests (including GPU tests)..." | tee -a "../$LOG_FILE"
ctest --output-on-failure >> "../$LOG_FILE" 2>&1

if [ $? -ne 0 ]; then
    echo "Warning: Some unit tests failed. Check log for details." | tee -a "../$LOG_FILE"
else
    echo "All unit tests passed!" | tee -a "../$LOG_FILE"
fi
cd ..

echo
echo "Step 3: Testing help functionality..." | tee -a "$LOG_FILE"
echo
./build/crossmon --help | tee -a "$LOG_FILE"

echo
echo "Step 4: Quick system test..." | tee -a "$LOG_FILE"
echo
bash scripts/test_quick_macos.sh "$LOG_FILE"

echo
echo "All tests completed!" | tee -a "$LOG_FILE"
echo "Results saved to: $LOG_FILE"
read -p "Press any key to continue..."
