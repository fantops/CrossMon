#pragma once
#include <string>
#include <vector>
#include "monitor_args.hpp"
#include "statistics.hpp"

struct SystemSamples {
    std::vector<double> cpuUsage;
    std::vector<uint64_t> memoryUsedMB;
    std::vector<double> memoryUsedPercent;
    std::vector<double> gpuUtilization;
    std::size_t gpuCount = 0;
};

void monitorSystemUsage(const MonitorArgs& args, SystemSamples& samples);
void outputSystemStatistics(const SystemSamples& samples, const std::string& resultPath);

// Legacy functions for backward compatibility
void monitorCpuUsage(const MonitorArgs& args, std::vector<double>& samples);
void outputStatistics(const std::vector<double>& samples, const std::string& resultPath); 