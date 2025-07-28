#pragma once
#include <string>
#include "statistics.hpp"

void printCpuStatsToConsole(const CpuStats& stats);
void writeCpuStatsToFile(const CpuStats& stats, const std::string& path);

void printMemoryStatsToConsole(const MemoryStats& stats);
void writeMemoryStatsToFile(const MemoryStats& stats, const std::string& path);

void printGpuStatsToConsole(const GpuStats& stats);
void writeGpuStatsToFile(const GpuStats& stats, const std::string& path);

void printNpuStatsToConsole(const NpuStats& stats);
void writeNpuStatsToFile(const NpuStats& stats, const std::string& path);

void printSystemStatsToConsole(const SystemStats& stats);
void writeSystemStatsToFile(const SystemStats& stats, const std::string& path); 