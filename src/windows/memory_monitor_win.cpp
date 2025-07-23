#include "memory_monitor.hpp"
#include <windows.h>
#include <psapi.h>

#pragma comment(lib, "psapi.lib")

class WindowsMemoryMonitor : public IMemoryMonitor {
public:
    WindowsMemoryMonitor() = default;
    
    MemoryUsage getMemoryUsage() override {
        MemoryUsage usage = {};
        
        // Get global memory status
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        
        if (GlobalMemoryStatusEx(&memInfo)) {
            // Physical memory (RAM)
            usage.totalPhysicalMB = memInfo.ullTotalPhys / (1024 * 1024);
            usage.availablePhysicalMB = memInfo.ullAvailPhys / (1024 * 1024);
            usage.usedPhysicalMB = usage.totalPhysicalMB - usage.availablePhysicalMB;
            usage.usedPercentage = (double)usage.usedPhysicalMB / usage.totalPhysicalMB * 100.0;
            
            // Virtual memory
            usage.totalVirtualMB = memInfo.ullTotalVirtual / (1024 * 1024);
            usage.availableVirtualMB = memInfo.ullAvailVirtual / (1024 * 1024);
            usage.usedVirtualMB = usage.totalVirtualMB - usage.availableVirtualMB;
        }
        
        return usage;
    }
    
    bool isSupported() const override {
        return true; // Always supported on Windows
    }
};

// Factory function for use elsewhere
IMemoryMonitor* createMemoryMonitor() {
    return new WindowsMemoryMonitor();
}
