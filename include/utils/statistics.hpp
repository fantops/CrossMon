#pragma once
#include <vector>
#include <cstddef>
#include <cstdint>
#include <string>

struct CpuStats {
    std::size_t samples = 0;
    double peak = 0.0;
    double average = 0.0;
    double min = 0.0;
    double max = 0.0;
};

struct MemoryStats {
    std::size_t samples = 0;
    uint64_t peakUsedMB = 0;
    uint64_t avgUsedMB = 0;
    uint64_t minUsedMB = 0;
    uint64_t maxUsedMB = 0;
    double peakUsedPercent = 0.0;
    double avgUsedPercent = 0.0;
    double minUsedPercent = 0.0;
    double maxUsedPercent = 0.0;
};

struct GpuStats {
    std::size_t samples = 0;
    double peakUtilization = 0.0;
    double avgUtilization = 0.0;
    double minUtilization = 0.0;
    double maxUtilization = 0.0;
    std::size_t gpuCount = 0;
};

struct NpuStats {
    std::size_t samples = 0;
    double peakUtilization = 0.0;
    double avgUtilization = 0.0;
    double minUtilization = 0.0;
    double maxUtilization = 0.0;
    std::string npuName = "";
};

struct SystemStats {
    CpuStats cpu;
    MemoryStats memory;
    GpuStats gpu;
    NpuStats npu;
};

CpuStats computeCpuStats(const std::vector<double>& values);
MemoryStats computeMemoryStats(const std::vector<uint64_t>& usedMB, const std::vector<double>& usedPercent);
GpuStats computeGpuStats(const std::vector<double>& utilization, std::size_t gpuCount);
NpuStats computeNpuStats(const std::vector<double>& utilization, const std::string& npuName);
double computePeak(const std::vector<double>& values);
double computeAverage(const std::vector<double>& values);
double computeMin(const std::vector<double>& values);
double computeMax(const std::vector<double>& values); 