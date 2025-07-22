# CrossMon

CrossMon is a lightweight, cross-platform C++17 console tool for real-time system monitoring. It reports CPU usage (with planned support for memory, GPU, and NPU) and outputs results in both human-readable and machine-friendly formats.

## Features
- ✅ **CPU Monitoring**: Real-time CPU utilization tracking
- ✅ **Memory Monitoring**: Physical memory usage in MB and percentage
- ✅ **GPU Monitoring**: Multi-GPU detection and utilization (Windows: DXGI/NVIDIA/Intel/AMD, macOS: IOKit)
- ✅ **Cross-Platform**: Windows and macOS support
- ✅ **Application-Specific Monitoring**: Monitor system usage while specific apps run
- ✅ **Flexible Output**: Console display + file output with statistics
- ✅ **Customizable Sampling**: Adjustable monitoring intervals
- ✅ **Graceful Shutdown**: Clean exit with Ctrl+C and comprehensive statistics
- ✅ **Modern C++17**: Modular, extensible, and well-tested codebase

## Flow Diagram
![alt text](data/images/Block_Diagram.jpeg)

## High-Level Design (HLD)

CrossMon is structured in a layered, modular architecture:

```
┌───────────────────────────────┐
│ Command-Line Interface (CLI)  │  ← parses flags, prints output
├───────────────────────────────┤
│ Monitor Controller            │  ← orchestrates timing & aggregation
├───────────────────────────────┤
│ Platform-Agnostic API Layer   │  ← pure C++ interfaces
├─────────┬─────────┬───────────┤
│ CPU API │ GPU API │ NPU API   │  ← abstract base classes
├─────────┼─────────┼───────────┤
│ OS-Specific Adaptors          │  ← Windows / macOS impls
└─────────┴─────────┴───────────┘
```

- **CLI Manager**: Parses arguments, builds config, formats output
- **Monitor Controller**: Initializes monitors, schedules sampling loop
- **CPU Monitor**: Calculates CPU-busy delta between two snapshots
- **Data Formatter**: Converts metrics to JSON or text
- **Process Manager**: Launches and checks app status
- **Statistics Module**: Computes peak, average, min, max

## Quick Start with Test Scripts

For easy testing, use the provided test scripts in the `scripts/` folder:

### Windows
```cmd
# Interactive test menu
scripts\run_tests_windows.bat

# Or run specific tests
scripts\build_and_test_windows.bat     # Build and test everything
scripts\test_quick_windows.bat         # 15-second system monitor test
scripts\test_gpu_detection_windows.bat # Test GPU detection
```

### macOS
```bash
# Make scripts executable (first time only)
chmod +x scripts/*.sh

# Interactive test menu
scripts/run_tests_macos.sh

# Or run specific tests
scripts/build_and_test_macos.sh       # Build and test everything
scripts/test_quick_macos.sh           # 15-second system monitor test  
scripts/test_gpu_detection_macos.sh   # Test GPU detection
```

## Build Instructions

### Windows (Visual Studio)
1. **Prerequisites:**
   - Visual Studio 2019 or later with C++ support
   - CMake 3.16 or later

2. **Clone and build:**
   ```cmd
   git clone https://github.com/fantops/CrossMon.git
   cd CrossMon
   cmake -B build -G "Visual Studio 16 2019"
   cmake --build build --config Release
   ```

### macOS/Linux
1. **Clone the repository:**
   ```sh
   git clone https://github.com/fantops/CrossMon.git
   cd CrossMon
   ```
2. **Configure and build:**
   ```sh
   cmake -B build
   cmake --build build
   ```

## Usage

### Windows Examples
Monitor a specific app (e.g., Notepad) every 500ms:
```cmd
build\Release\crossmon.exe -i 500 "notepad.exe"
```

Monitor system CPU usage (no app) until Ctrl+C:
```cmd
build\Release\crossmon.exe -i 1000
```

Output statistics to a file:
```cmd
build\Release\crossmon.exe -i 500 -o results.txt "chrome.exe"
```

### macOS Examples
Monitor a specific app (e.g., Photo Booth) every 500ms:
```sh
./build/crossmon -i 500 "Photo Booth"
```

Monitor system CPU usage (no app) until Ctrl+C:
```sh
./build/crossmon -i 1000
```

Output statistics to a file:
```sh
./build/crossmon -i 500 -o results.txt "Safari"
```

## Output
- Prints live CPU usage to the console
- On exit, prints and (optionally) writes statistics: samples, peak, average, min, max

## Modular Structure
- `include/utils/` and `src/utils/`: argument parsing, monitoring, process management, statistics, output formatting
- `src/monitors/`: (planned) platform-specific monitor implementations
- `test/`: unit tests for all major modules

## Run Unit Tests

### Windows
```cmd
cd build
ctest -C Release
```

### macOS/Linux
```sh
cd build
ctest
```
Or run individual test executables in `build/` (Windows: `build/Release/`).

## Planned Extensions
- Memory, GPU, and NPU monitoring
- JSON output
- Linux support (Windows and macOS currently supported)

---

**CrossMon** is designed for extensibility and reliability. Contributions and suggestions are welcome!
