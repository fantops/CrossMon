#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "cpu_monitor.hpp"
#include <cstdlib> // for system()
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "utils/monitor_args.hpp"
#include "utils/monitor_utils.hpp"
#include "utils/process_manager.hpp"

ICpuMonitor* createCpuMonitor();

int main(int argc, char* argv[]) {
    // Parse arguments
    MonitorArgs args = parseMonitorArgs(argc, argv);

    // Launch the app
    launchApp(args.appName);

    // Monitor CPU usage
    std::vector<double> samples;
    monitorCpuUsage(args, samples);

    // Output statistics
    outputStatistics(samples, args.resultPath);

    return 0;
} 