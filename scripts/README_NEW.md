# CrossMon Test Scripts

This folder contains test scripts to help users build, test, and validate CrossMon functionality on different platforms. All scripts automatically log their output to the `results/` directory.

## Windows Scripts

### `build_and_test_windows.bat`
Complete build and test pipeline that:
- Configures and builds CrossMon using CMake
- Runs all unit tests
- Performs functional validation tests
- Logs all output to `results/build_and_test.log`

### `test_quick_windows.bat`
Quick validation test that:
- Builds the project if needed
- Runs a 15-second system monitoring test
- Verifies basic CPU, Memory, and GPU monitoring functionality

### `test_gpu_detection_windows.bat`
Focused GPU testing that:
- Tests GPU detection and enumeration
- Validates GPU utilization monitoring
- Compares results with Windows Task Manager
- Useful for troubleshooting GPU monitoring issues

### `test_app_monitoring_windows.bat`
Application monitoring test that:
- Launches a test application (notepad.exe)
- Monitors system resources while the app runs
- Demonstrates application-specific monitoring features

### `run_tests_windows.bat`
Unit test runner that:
- Builds and runs all unit tests including new GPU monitor tests
- Tests CPU monitoring, GPU monitoring, statistics, and utilities
- Provides detailed test results for all components
- Validates Windows-specific GPU WMI implementation
- Useful for development and validation

## macOS Scripts

### `build_and_test_macos.sh`
Complete build and test pipeline for macOS that:
- Configures and builds CrossMon using CMake and make
- Runs all unit tests
- Performs functional validation tests
- Logs all output to `results/build_and_test.log`

### `test_quick_macos.sh`
Quick validation test for macOS

### `test_gpu_detection_macos.sh`
GPU testing for macOS (IOKit-based)

### `test_app_monitoring_macos.sh`
Application monitoring test for macOS

### `run_tests_macos.sh`
Unit test runner for macOS that:
- Builds and runs all unit tests including new GPU monitor tests
- Tests CPU monitoring, GPU monitoring, statistics, and utilities
- Validates macOS-specific IOKit implementation
- Provides comprehensive test coverage for all components

## Results Directory
All scripts log their output to the `results/` directory:
```
results/
├── build_and_test.log
├── test_quick.log
├── gpu_detection.log
├── app_monitoring.log
├── unit_tests.log       # Includes GPU unit test results
```

## Usage Examples

### Windows
```cmd
# Complete build and test
scripts\build_and_test_windows.bat

# Quick functionality check
scripts\test_quick_windows.bat

# Test GPU monitoring specifically
scripts\test_gpu_detection_windows.bat

# Test application monitoring
scripts\test_app_monitoring_windows.bat
```

### macOS
```bash
# Complete build and test
scripts/build_and_test_macos.sh

# Quick functionality check
scripts/test_quick_macos.sh

# Test GPU monitoring specifically
scripts/test_gpu_detection_macos.sh
```

## Requirements

### Windows
- Visual Studio 2019 or later (or Build Tools)
- CMake 3.16+
- Git

### macOS
- Xcode Command Line Tools
- CMake 3.16+
- Git

## Troubleshooting

If you encounter build errors:
1. Check that all prerequisites are installed
2. Try running the build commands manually:
   - Windows: `cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --config Release`
   - macOS: `mkdir -p build && cd build && cmake .. && make`
3. Check the log files in the `results/` directory for detailed error information

## For Developers

These scripts are particularly useful for:
- **CI/CD Integration**: Use `build_and_test_*` scripts in automated pipelines
- **Feature Validation**: Use specific test scripts to validate new features
- **Cross-Platform Testing**: Ensure functionality works across Windows and macOS
- **GPU Troubleshooting**: Use GPU-specific tests to debug monitoring issues
- **Performance Testing**: Monitor resource usage during different scenarios
- **Unit Testing**: Comprehensive test coverage including new GPU monitor unit tests

### GPU Unit Test Coverage
The project now includes dedicated unit tests for GPU monitoring:

**Windows GPU Tests** (`test_gpu_monitor_win.cpp`):
- WMI GPU enumeration and detection
- GPU utilization value validation (0-100%)
- Multi-GPU support testing
- WMI COM interface availability checks
- Performance and stability testing
- Error handling and edge cases

**macOS GPU Tests** (`test_gpu_monitor_mac.cpp`):
- IOKit GPU detection (placeholder for future implementation)
- GPU count consistency checks
- Apple Silicon GPU detection
- Framework availability validation
- Cross-platform interface compliance

Run GPU-specific unit tests:
```cmd
# Windows - Run GPU unit tests specifically
cd build && ctest -R GpuMonitorWinTest -V

# macOS - Run GPU unit tests specifically  
cd build && ctest -R GpuMonitorMacTest -V
```

All scripts automatically create the `results/` directory and log detailed output for analysis.

## Production Ready Features

CrossMon now includes:
- ✅ **Multi-GPU Support**: Detects and monitors multiple GPUs individually
- ✅ **Debug Mode**: Use `build_debug.bat` for detailed diagnostic output
- ✅ **Configurable Sampling**: Adjust monitoring intervals with `-i` parameter
- ✅ **File Output**: Save statistics to files with `-o` parameter
- ✅ **Application Monitoring**: Monitor resources while specific apps run
- ✅ **Clean Architecture**: Platform-specific implementations with shared interfaces

Example usage with custom sampling:
```cmd
# Monitor every 500ms and save to file
crossmon.exe -i 500 -o results.txt notepad.exe

# Debug mode with detailed output
build_debug\Debug\crossmon.exe -i 1000
```
