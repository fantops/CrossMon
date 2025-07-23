#include "memory_monitor.hpp"
#include <mach/mach.h>
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>

class MacMemoryMonitor : public IMemoryMonitor {
public:
    MacMemoryMonitor() = default;
    
    MemoryUsage getMemoryUsage() override {
        MemoryUsage usage = {};
        
        vm_size_t page_size;
        vm_statistics64_data_t vm_stat;
        mach_msg_type_number_t host_size = sizeof(vm_statistics64_data_t) / sizeof(natural_t);
        
        // Get page size
        host_page_size(mach_host_self(), &page_size);
        
        // Get VM statistics
        if (host_statistics64(mach_host_self(), HOST_VM_INFO64, (host_info64_t)&vm_stat, &host_size) == KERN_SUCCESS) {
            // Calculate memory usage
            uint64_t total_pages = vm_stat.free_count + vm_stat.active_count + vm_stat.inactive_count + vm_stat.wire_count;
            uint64_t used_pages = vm_stat.active_count + vm_stat.inactive_count + vm_stat.wire_count;
            
            usage.totalPhysicalMB = (total_pages * page_size) / (1024 * 1024);
            usage.usedPhysicalMB = (used_pages * page_size) / (1024 * 1024);
            usage.availablePhysicalMB = usage.totalPhysicalMB - usage.usedPhysicalMB;
            usage.usedPercentage = (double)usage.usedPhysicalMB / usage.totalPhysicalMB * 100.0;
            
            // Virtual memory info (simplified for macOS)
            usage.totalVirtualMB = usage.totalPhysicalMB; // Simplified
            usage.usedVirtualMB = usage.usedPhysicalMB;
            usage.availableVirtualMB = usage.availablePhysicalMB;
        }
        
        return usage;
    }
    
    bool isSupported() const override {
        return true; // Always supported on macOS
    }
};

// Factory function for use elsewhere
IMemoryMonitor* createMemoryMonitor() {
    return new MacMemoryMonitor();
}
