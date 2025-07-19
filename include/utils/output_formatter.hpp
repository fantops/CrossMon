#pragma once
#include <string>
#include "statistics.hpp"

void printCpuStatsToConsole(const CpuStats& stats);
void writeCpuStatsToFile(const CpuStats& stats, const std::string& path); 