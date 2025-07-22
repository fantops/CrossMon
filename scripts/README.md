# CrossMon Test Scripts

This folder contains test scripts to help users easily build and test CrossMon on different platforms. All scripts now automatically log their output to the `results` folder.

## Windows Scripts (.bat)

### `build_and_test_windows.bat`
Complete build and test script that:
- Configures and builds CrossMon using CMake
- Tests help functionality
- Runs a quick system monitor test
- Logs all output to `results/build_and_test.log`

### `test_quick_windows.bat`
Quick 15-second system monitoring test to verify basic functionality.
- Can be run standalone or called from other scripts
- Logs output to `results/test_quick.log`

### `test_gpu_detection_windows.bat`
Tests GPU detection and enumeration. Compares results with Windows Task Manager.
- Logs output to `results/gpu_detection.log`
- Saves raw GPU output to `results/gpu_detection_output.txt`

### `test_app_monitoring_windows.bat`
Tests application-specific monitoring using Notepad as a test application.
- Logs output to `results/app_monitoring.log`
- Saves monitoring data to `results/notepad_monitoring.txt`

### `run_tests_windows.bat`
Interactive menu launcher for all Windows tests with easy access to results folder.

## macOS Scripts (.sh)

### `build_and_test_macos.sh`
Complete build and test script that:
- Configures and builds CrossMon using CMake and make
- Tests help functionality  
- Runs a quick system monitor test
- Logs all output to `results/build_and_test.log`

### `test_quick_macos.sh`
Quick 15-second system monitoring test to verify basic functionality.
- Can be run standalone or called from other scripts
- Logs output to `results/test_quick.log`

### `test_gpu_detection_macos.sh`
Tests GPU detection and enumeration. Compares results with Activity Monitor.
- Logs output to `results/gpu_detection.log`

### `test_app_monitoring_macos.sh`
Tests application-specific monitoring using TextEdit as a test application.
- Logs output to `results/app_monitoring.log`

### `run_tests_macos.sh`
Interactive menu launcher for all macOS tests with easy access to results folder.

## Results Folder

All scripts now automatically create and use a `results` folder in the project root to store:
- **Log files**: Complete script execution logs
- **Output files**: Raw monitoring data and test results

The results folder structure:
```
results/
├── build_and_test.log
├── test_quick.log
├── gpu_detection.log
├── gpu_detection_output.txt
├── app_monitoring.log
└── notepad_monitoring.txt
```

## Usage

### Windows
```cmd
# Run from project root directory
scripts\build_and_test_windows.bat

# Or run individual tests
scripts\test_quick_windows.bat
scripts\test_gpu_detection_windows.bat
scripts\test_app_monitoring_windows.bat
```

### macOS
```bash
# Make scripts executable (first time only)
chmod +x scripts/*.sh

# Run from project root directory
scripts/build_and_test_macos.sh

# Or run individual tests
scripts/test_quick_macos.sh
scripts/test_gpu_detection_macos.sh  
scripts/test_app_monitoring_macos.sh
```

## Prerequisites

### Windows
- Visual Studio 2019 or later (or Build Tools)
- CMake 3.16 or later
- Windows 10 or later

### macOS
- Xcode Command Line Tools
- CMake 3.16 or later
- macOS 10.15 or later

## Expected Output

All scripts will:
1. Display system information including detected CPUs, memory, and GPUs
2. Show real-time monitoring data
3. Generate statistics at the end
4. **Save all results to timestamped files in the results folder**
5. Provide clear paths to log files and output data

## Troubleshooting

If you encounter build errors:
1. Ensure all prerequisites are installed
2. Try running the build commands manually:
   - Windows: `cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --config Release`
   - macOS: `mkdir -p build && cd build && cmake .. && make`
3. **Check the log files in the results folder for detailed error information**

If GPU detection shows 0 GPUs:
- Windows: Check if your graphics drivers are up to date
- macOS: Some older Macs may not support GPU monitoring through IOKit
- **Review the GPU detection log files for debugging information**

For application monitoring issues:
- Ensure the target application name is correct
- Try using full executable names (e.g., "notepad.exe" instead of "notepad")
- **Check the application monitoring log files for detailed execution traces**

## Log File Management

The results folder will contain the following files that get overwritten on each test run:
- Use the test launcher's "View Results Folder" option to browse files
- Log files are overwritten each time you run the same test
- Save important results before running tests again if you need to keep them
