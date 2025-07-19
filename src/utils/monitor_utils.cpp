#include "utils/monitor_utils.hpp"
#include "utils/process_manager.hpp"
#include "utils/statistics.hpp"
#include "utils/output_formatter.hpp"
#include "cpu_monitor.hpp"
#include <thread>
#include <chrono>
#include <vector>
#include <iostream>
#include <csignal>
#include <atomic>

ICpuMonitor* createCpuMonitor();

namespace {
std::atomic<bool> keepRunning(true);
void signalHandler(int) { keepRunning = false; }
}

void monitorCpuUsage(const MonitorArgs& args, std::vector<double>& samples) {
    std::signal(SIGINT, signalHandler);
    ICpuMonitor* monitor = createCpuMonitor();
    monitor->getCpuBusy(); // Initialize
    if (!args.appName.empty()) {
        while (keepRunning && isAppRunning(args.appName)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(args.interval));
            double cpu = monitor->getCpuBusy();
            samples.push_back(cpu);
            std::cout << "CPU Busy: " << cpu << "%" << std::endl;
        }
    } else {
        while (keepRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(args.interval));
            double cpu = monitor->getCpuBusy();
            samples.push_back(cpu);
            std::cout << "CPU Busy: " << cpu << "%" << std::endl;
        }
    }
    delete monitor;
}

void outputStatistics(const std::vector<double>& samples, const std::string& resultPath) {
    CpuStats stats = computeCpuStats(samples);
    printCpuStatsToConsole(stats);
    if (!resultPath.empty()) {
        writeCpuStatsToFile(stats, resultPath);
    }
} 