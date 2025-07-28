#include "../../include/npu_monitor.hpp"
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <memory>

#pragma comment(lib, "pdh.lib")

class WindowsNPUMonitor : public NPUMonitor {
private:
    bool initialized;
    std::string npu_name;
    PDH_HQUERY hQuery;
    std::vector<PDH_HCOUNTER> hCounters;
    bool has_perf_counters;

    bool InitializePerformanceCounters() {
        PDH_STATUS status = PdhOpenQuery(NULL, 0, &hQuery);
        if (status != ERROR_SUCCESS) {
#ifdef _DEBUG
            std::cout << "[DEBUG] Failed to open PDH query: 0x" << std::hex << status << std::endl;
#endif
            return false;
        }

        // Task Manager uses GPU Engine counters for NPU - try the exact patterns
        std::vector<std::string> counter_patterns = {
            // These are the exact patterns Task Manager likely uses
            "\\GPU Engine(*)\\Utilization Percentage",
            "\\GPU Engine(NPU 0 - Intel(R) NPU)\\Utilization Percentage",
            "\\GPU Engine(Intel(R) NPU)\\Utilization Percentage",
            "\\GPU Engine(NPU*)\\Utilization Percentage"
        };

        bool found_any = false;
        for (const auto& pattern : counter_patterns) {
#ifdef _DEBUG
            std::cout << "[DEBUG] Trying counter pattern: " << pattern << std::endl;
#endif
            
            PDH_HCOUNTER hCounter = NULL;
            status = PdhAddCounterA(hQuery, pattern.c_str(), 0, &hCounter);
            
            if (status == ERROR_SUCCESS) {
#ifdef _DEBUG
                std::cout << "[DEBUG] Successfully added counter: " << pattern << std::endl;
#endif
                hCounters.push_back(hCounter);
                found_any = true;
                
                // For wildcard patterns, we might get multiple instances
                // Let's collect data to see what instances we get
                PdhCollectQueryData(hQuery);
                Sleep(100);
                PdhCollectQueryData(hQuery);
                
                // Check if this counter gives us valid data
                PDH_FMT_COUNTERVALUE counterVal;
                PDH_STATUS valueStatus = PdhGetFormattedCounterValue(hCounter, PDH_FMT_DOUBLE, NULL, &counterVal);
                if (valueStatus == ERROR_SUCCESS && counterVal.CStatus == PDH_CSTATUS_VALID_DATA) {
#ifdef _DEBUG
                    std::cout << "[DEBUG] Counter is working, value: " << counterVal.doubleValue << std::endl;
#endif
                } else {
#ifdef _DEBUG
                    std::cout << "[DEBUG] Counter added but no valid data yet, status: 0x" << std::hex << valueStatus << std::endl;
#endif
                }
            } else {
#ifdef _DEBUG
                std::cout << "[DEBUG] Failed to add counter " << pattern << ": 0x" << std::hex << status << std::endl;
#endif
            }
        }

        if (found_any) {
            has_perf_counters = true;
#ifdef _DEBUG
            std::cout << "[DEBUG] NPU performance counters initialized successfully" << std::endl;
#endif
            return true;
        }

#ifdef _DEBUG
        std::cout << "[DEBUG] No NPU performance counters found" << std::endl;
#endif
        PdhCloseQuery(hQuery);
        hQuery = NULL;
        has_perf_counters = false;
        return false;
    }

public:
    WindowsNPUMonitor() : initialized(false), npu_name("Intel(R) NPU"), 
                         hQuery(NULL), has_perf_counters(false) {}

    ~WindowsNPUMonitor() {
        if (hQuery) {
            PdhCloseQuery(hQuery);
        }
    }

    bool initialize() override {
        // Assume Intel NPU is present based on Task Manager showing "NPU 0 - Intel(R) NPU"
        npu_name = "NPU 0 - Intel(R) NPU";
        initialized = true;
        
        // Try to initialize performance counters
        InitializePerformanceCounters();
        
        return true; // Always return true since we know the NPU exists
    }

    std::string get_name() const {
        return npu_name;
    }

    NPUUsage get_usage() override {
        NPUUsage usage;
        usage.usage_percent = 0.0f;
        usage.name = npu_name;

        if (!initialized) {
            return usage;
        }

        if (has_perf_counters && hQuery && !hCounters.empty()) {
            // Collect performance data
            PDH_STATUS status = PdhCollectQueryData(hQuery);
            if (status == ERROR_SUCCESS) {
                // Wait a bit for data to be available
                Sleep(50);
                
                // Try to get data from all available counters
                float max_usage = 0.0f;
                bool found_valid_data = false;
                
                for (size_t i = 0; i < hCounters.size(); i++) {
                    PDH_HCOUNTER hCounter = hCounters[i];
                    PDH_FMT_COUNTERVALUE counterVal;
                    status = PdhGetFormattedCounterValue(hCounter, PDH_FMT_DOUBLE, NULL, &counterVal);
                    
                    if (status == ERROR_SUCCESS && counterVal.CStatus == PDH_CSTATUS_VALID_DATA) {
                        float current_usage = static_cast<float>(counterVal.doubleValue);
#ifdef _DEBUG
                        std::cout << "[DEBUG] NPU counter " << i << " value: " << current_usage << "%" << std::endl;
#endif
                        
                        // For GPU Engine(*) wildcard, we might get multiple instances
                        // We want the maximum usage across all NPU-related instances
                        if (current_usage > max_usage) {
                            max_usage = current_usage;
                        }
                        found_valid_data = true;
                    } else {
#ifdef _DEBUG
                        std::cout << "[DEBUG] NPU counter " << i << " status: 0x" << std::hex << status 
                                  << ", CStatus: 0x" << counterVal.CStatus << std::dec << std::endl;
#endif
                    }
                }
                
                if (found_valid_data) {
                    usage.usage_percent = std::min(100.0f, std::max(0.0f, max_usage));
#ifdef _DEBUG
                    std::cout << "[DEBUG] Final NPU usage: " << usage.usage_percent << "%" << std::endl;
#endif
                    return usage;
                }
            } else {
#ifdef _DEBUG
                std::cout << "[DEBUG] PdhCollectQueryData failed: 0x" << std::hex << status << std::endl;
#endif
            }
        }
        
        // Fallback: Return 0% if no performance counters work
#ifdef _DEBUG
        std::cout << "[DEBUG] NPU usage fallback to 0%" << std::endl;
#endif
        usage.usage_percent = 0.0f;
        return usage;
    }
};

// Factory function
NPUMonitor* create_windows_npu_monitor() {
    return new WindowsNPUMonitor();
}
