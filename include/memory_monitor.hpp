#pragma once
#include <cstdint>

struct MemoryUsage {
    uint64_t totalPhysicalMB;    // Total physical RAM in MB
    uint64_t availablePhysicalMB; // Available physical RAM in MB
    uint64_t usedPhysicalMB;     // Used physical RAM in MB
    double usedPercentage;       // Used memory as percentage
    
    uint64_t totalVirtualMB;     // Total virtual memory in MB
    uint64_t availableVirtualMB; // Available virtual memory in MB
    uint64_t usedVirtualMB;      // Used virtual memory in MB
};

class IMemoryMonitor {
public:
    virtual ~IMemoryMonitor() = default;
    
    // Returns current memory usage statistics
    virtual MemoryUsage getMemoryUsage() = 0;
    
    // Returns true if this monitor is supported on the current platform
    virtual bool isSupported() const = 0;
};
