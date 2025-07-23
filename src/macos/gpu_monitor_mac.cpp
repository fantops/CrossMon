#include "gpu_monitor.hpp"
#include <IOKit/IOKitLib.h>
#include <CoreFoundation/CoreFoundation.h>
#include <string>
#include <vector>

class MacGpuMonitor : public IGpuMonitor {
public:
    MacGpuMonitor() : initialized(false), gpuCount(0) {
        initialize();
    }
    
    ~MacGpuMonitor() {
        cleanup();
    }
    
    GpuUsage getGpuUsage() override {
        GpuUsage usage = {};
        
        if (!initialized) {
            return usage;
        }
        
        // Query IOKit for GPU information
        io_iterator_t iterator;
        kern_return_t result = IOServiceGetMatchingServices(kIOMasterPortDefault,
            IOServiceMatching("IOPCIDevice"), &iterator);
        
        if (result != KERN_SUCCESS) {
            return usage;
        }
        
        io_registry_entry_t regEntry;
        while ((regEntry = IOIteratorNext(iterator))) {
            CFMutableDictionaryRef properties;
            result = IORegistryEntryCreateCFProperties(regEntry, &properties,
                kCFAllocatorDefault, kNilOptions);
            
            if (result == KERN_SUCCESS) {
                // Check if this is a GPU device
                CFStringRef deviceName = (CFStringRef)CFDictionaryGetValue(properties, CFSTR("model"));
                CFStringRef className = (CFStringRef)CFDictionaryGetValue(properties, CFSTR("IOClass"));
                
                if (deviceName && className) {
                    char nameBuffer[256] = {0};
                    char classBuffer[256] = {0};
                    
                    CFStringGetCString(deviceName, nameBuffer, sizeof(nameBuffer), kCFStringEncodingUTF8);
                    CFStringGetCString(className, classBuffer, sizeof(classBuffer), kCFStringEncodingUTF8);
                    
                    // Look for GPU-related classes or names
                    std::string name(nameBuffer);
                    std::string deviceClass(classBuffer);
                    
                    if (isGpuDevice(name, deviceClass)) {
                        GpuInfo gpu;
                        gpu.name = name;
                        
                        // Simplified utilization (macOS doesn't provide easy GPU utilization access)
                        gpu.utilizationPercent = 0.0; // Would need Metal Performance Shaders or Activity Monitor APIs
                        gpu.temperature = 0;
                        gpu.powerUsage = 0;
                        
                        usage.gpus.push_back(gpu);
                    }
                }
                
                CFRelease(properties);
            }
            
            IOObjectRelease(regEntry);
        }
        
        IOObjectRelease(iterator);
        
        gpuCount = usage.gpus.size();
        
        // Calculate average utilization
        if (!usage.gpus.empty()) {
            double totalUtil = 0.0;
            for (const auto& gpu : usage.gpus) {
                totalUtil += gpu.utilizationPercent;
            }
            usage.averageUtilization = totalUtil / usage.gpus.size();
        }
        
        return usage;
    }
    
    bool isSupported() const override {
        return initialized;
    }
    
    size_t getGpuCount() const override {
        return gpuCount;
    }

private:
    bool initialized;
    size_t gpuCount;
    
    void initialize() {
        initialized = true; // IOKit is always available on macOS
    }
    
    void cleanup() {
        // Nothing to cleanup for IOKit
    }
    
    bool isGpuDevice(const std::string& name, const std::string& deviceClass) {
        // Check for common GPU identifiers
        std::string lowerName = name;
        std::string lowerClass = deviceClass;
        
        // Convert to lowercase for comparison
        for (auto& c : lowerName) c = std::tolower(c);
        for (auto& c : lowerClass) c = std::tolower(c);
        
        return (lowerName.find("gpu") != std::string::npos ||
                lowerName.find("radeon") != std::string::npos ||
                lowerName.find("nvidia") != std::string::npos ||
                lowerName.find("geforce") != std::string::npos ||
                lowerName.find("quadro") != std::string::npos ||
                lowerName.find("intel") != std::string::npos ||
                lowerClass.find("gpu") != std::string::npos ||
                lowerClass.find("display") != std::string::npos);
    }
};

// Factory function for use elsewhere
IGpuMonitor* createGpuMonitor() {
    return new MacGpuMonitor();
}
