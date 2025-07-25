# CrossMon Scripts

> For full requirements and build instructions, see the main project [README.md](../README.md).

This folder contains scripts for building, testing, and validating CrossMon on both Windows and macOS.

## Unified Build and Test Scripts

### Windows
- **`build_and_test_windows.bat`**
  - Usage: `build_and_test_windows.bat [release|debug]`
  - Builds the project, runs all unit tests, and performs a quick 15-second system monitor test.
  - Logs are saved to `results/build_and_test_<build_type>.log`.

### macOS
- **`build_and_test_macos.sh`**
  - Usage: `./build_and_test_macos.sh [release|debug]`
  - Same as above for macOS.

## Other Scripts
- **run_tests_macos.sh / run_tests_windows.bat**: Interactive test menus.
- **test_app_monitoring_*.sh/.bat, test_gpu_detection_*.sh/.bat**: Specialized tests for app monitoring and GPU detection.

## Notes
- Make sure you have the required dependencies installed for your platform.
