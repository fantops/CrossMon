# CrossMon Scripts

This folder contains scripts for building, testing, and validating CrossMon on both Windows and macOS.

## Unified Build and Test Scripts

### Windows
- **`build_and_test_windows.bat`**
  - Usage: `build_and_test_windows.bat [release|debug]`
  - Builds the project (Release or Debug), runs all unit tests, and performs a quick 15-second system monitor test.
  - Example:
    ```cmd
    build_and_test_windows.bat release
    build_and_test_windows.bat debug
    ```
  - Output logs are saved to `results/build_and_test_<build_type>.log`.

### macOS
- **`build_and_test_macos.sh`**
  - Usage: `./build_and_test_macos.sh [release|debug]`
  - Builds the project (Release or Debug), runs all unit tests, and performs a quick 15-second system monitor test.
  - Example:
    ```sh
    ./build_and_test_macos.sh release
    ./build_and_test_macos.sh debug
    ```
  - Output logs are saved to `results/build_and_test_<build_type>.log`.

## Other Scripts
- **run_tests_macos.sh / run_tests_windows.bat**: Interactive test menus for each platform.
- **test_app_monitoring_*.sh/.bat, test_gpu_detection_*.sh/.bat**: Specialized tests for app monitoring and GPU detection.

## Notes
- Make sure you have the required dependencies (CMake, compilers, etc.) installed for your platform.
- On macOS, install GNU coreutils (`brew install coreutils`) for `gtimeout` support.
