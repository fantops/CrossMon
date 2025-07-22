#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct GpuInfo {
    std::string name;           // GPU name/model
    double utilizationPercent;  // GPU utilization percentage
    uint32_t temperature;       // GPU temperature in Celsius (if available)
    uint32_t powerUsage;        // Power usage in watts (if available)
};

struct GpuUsage {
    std::vector<GpuInfo> gpus;  // Support for multiple GPUs
    double averageUtilization;  // Average across all GPUs
};

class IGpuMonitor {
public:
    virtual ~IGpuMonitor() = default;
    
    // Returns current GPU usage statistics
    virtual GpuUsage getGpuUsage() = 0;
    
    // Returns true if this monitor is supported on the current platform
    virtual bool isSupported() const = 0;
    
    // Returns number of GPUs detected
    virtual size_t getGpuCount() const = 0;
};
