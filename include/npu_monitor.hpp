#pragma once
#ifdef _WIN32
#include <string>
#endif

struct NPUUsage {
    float usage_percent;
    std::string name;
};

class NPUMonitor {
public:
    virtual ~NPUMonitor() = default;
    virtual bool initialize() = 0;
    virtual NPUUsage get_usage() = 0;
};

#ifdef _WIN32
NPUMonitor* create_windows_npu_monitor();
#endif 