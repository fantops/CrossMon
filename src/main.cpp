#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>
#include <cstdlib> // for system()
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "cpu_monitor.hpp"
#include "memory_monitor.hpp"
#include "gpu_monitor.hpp"
#include "utils/monitor_args.hpp"
#include "utils/monitor_utils.hpp"
#include "utils/process_manager.hpp"

// Platform-specific includes
#ifdef _WIN32
    // Windows-specific includes handled in implementation files
#else
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <unistd.h>
#endif

#ifdef _WIN32
#include "../include/npu_monitor.hpp"
#endif

ICpuMonitor* createCpuMonitor();
IMemoryMonitor* createMemoryMonitor();
IGpuMonitor* createGpuMonitor();

int main(int argc, char* argv[]) {
    // Parse arguments
    MonitorArgs args = parseMonitorArgs(argc, argv);
    
    // Handle help request
    if (args.showHelp) {
        printHelp(argv[0]);
        return 0;
    }
    
    // Handle argument errors
    if (args.hasError) {
        std::cerr << args.errorMessage << std::endl;
        std::cerr << "Use --help for usage information." << std::endl;
        return 1;
    }

    // Launch the app if specified
    if (!args.appName.empty()) {
        launchApp(args.appName);
    }

    // Monitor system usage (CPU + Memory + GPU)
    SystemSamples samples;
    monitorSystemUsage(args, samples);

    // Output statistics
    outputSystemStatistics(samples, args.resultPath);

#ifdef _WIN32
    NPUMonitor* npu_monitor = create_windows_npu_monitor();
    if (npu_monitor->initialize()) {
        NPUUsage npu = npu_monitor->get_usage();
        std::cout << "NPU: " << npu.name << " | Usage: " << npu.usage_percent << "%\n";
    } else {
        std::cout << "NPU monitoring not available." << std::endl;
    }
    delete npu_monitor;
#else
    std::cout << "NPU monitoring not supported on this platform." << std::endl;
#endif

    return 0;
} 