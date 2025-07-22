#include "utils/statistics.hpp"
#include <algorithm>
#include <numeric>

CpuStats computeCpuStats(const std::vector<double>& values) {
    CpuStats stats;
    stats.samples = values.size();
    if (values.empty()) return stats;
    stats.peak = *std::max_element(values.begin(), values.end());
    stats.average = std::accumulate(values.begin(), values.end(), 0.0) / values.size();
    stats.min = *std::min_element(values.begin(), values.end());
    stats.max = stats.peak;
    return stats;
}

MemoryStats computeMemoryStats(const std::vector<uint64_t>& usedMB, const std::vector<double>& usedPercent) {
    MemoryStats stats;
    stats.samples = usedMB.size();
    if (usedMB.empty() || usedPercent.empty()) return stats;
    
    // Memory usage in MB
    stats.peakUsedMB = *std::max_element(usedMB.begin(), usedMB.end());
    stats.avgUsedMB = std::accumulate(usedMB.begin(), usedMB.end(), 0ULL) / usedMB.size();
    stats.minUsedMB = *std::min_element(usedMB.begin(), usedMB.end());
    stats.maxUsedMB = stats.peakUsedMB;
    
    // Memory usage in percentage
    stats.peakUsedPercent = *std::max_element(usedPercent.begin(), usedPercent.end());
    stats.avgUsedPercent = std::accumulate(usedPercent.begin(), usedPercent.end(), 0.0) / usedPercent.size();
    stats.minUsedPercent = *std::min_element(usedPercent.begin(), usedPercent.end());
    stats.maxUsedPercent = stats.peakUsedPercent;
    
    return stats;
}

GpuStats computeGpuStats(const std::vector<double>& utilization, std::size_t gpuCount) {
    GpuStats stats;
    stats.samples = utilization.size();
    stats.gpuCount = gpuCount;
    
    if (utilization.empty()) {
        return stats;
    }
    
    // GPU utilization stats
    stats.peakUtilization = *std::max_element(utilization.begin(), utilization.end());
    stats.avgUtilization = std::accumulate(utilization.begin(), utilization.end(), 0.0) / utilization.size();
    stats.minUtilization = *std::min_element(utilization.begin(), utilization.end());
    stats.maxUtilization = stats.peakUtilization;
    
    return stats;
}

double computePeak(const std::vector<double>& values) {
    return values.empty() ? 0.0 : *std::max_element(values.begin(), values.end());
}

double computeAverage(const std::vector<double>& values) {
    return values.empty() ? 0.0 : std::accumulate(values.begin(), values.end(), 0.0) / values.size();
}

double computeMin(const std::vector<double>& values) {
    return values.empty() ? 0.0 : *std::min_element(values.begin(), values.end());
}

double computeMax(const std::vector<double>& values) {
    return values.empty() ? 0.0 : *std::max_element(values.begin(), values.end());
} 