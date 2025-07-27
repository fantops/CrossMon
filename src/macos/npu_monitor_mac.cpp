#include "../../include/npu_monitor.hpp"
#include <iostream>

class MacNPUMonitor : public NPUMonitor {
public:
    bool initialize() override { return false; }
    NPUUsage get_usage() override {
        NPUUsage usage;
        usage.usage_percent = 0.0f;
        usage.name = "NPU monitoring not supported on macOS";
        return usage;
    }
}; 