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

/**
 * Factory function for creating platform-specific GPU monitors.
 * 
 * This function creates and returns a pointer to a platform-specific implementation
 * of the IGpuMonitor interface. The actual implementation depends on the target platform:
 * - Windows: Uses WMI (Windows Management Instrumentation) and DXGI for GPU monitoring
 * - macOS: Uses IOKit framework for GPU information access
 * 
 * The caller is responsible for managing the lifetime of the returned monitor instance.
 * 
 * @return Pointer to a platform-specific IGpuMonitor implementation, or nullptr if 
 *         GPU monitoring is not supported on the current platform
 */
IGpuMonitor* createGpuMonitor();
