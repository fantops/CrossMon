#pragma once
#include <vector>
#include <cstddef>

struct CpuStats {
    std::size_t samples = 0;
    double peak = 0.0;
    double average = 0.0;
    double min = 0.0;
    double max = 0.0;
};

CpuStats computeCpuStats(const std::vector<double>& values);
double computePeak(const std::vector<double>& values);
double computeAverage(const std::vector<double>& values);
double computeMin(const std::vector<double>& values);
double computeMax(const std::vector<double>& values); 