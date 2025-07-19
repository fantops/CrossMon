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