#pragma once
#include <string>
#include <vector>
#include "monitor_args.hpp"

void monitorCpuUsage(const MonitorArgs& args, std::vector<double>& samples);
void outputStatistics(const std::vector<double>& samples, const std::string& resultPath); 