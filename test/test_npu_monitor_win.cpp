#include "../include/npu_monitor.hpp"
#include <cassert>
#include <iostream>

int main() {
#ifdef _WIN32
    NPUMonitor* monitor = create_windows_npu_monitor();
    assert(monitor->initialize());
    NPUUsage usage = monitor->get_usage();
    std::cout << "NPU Name: " << usage.name << ", Usage: " << usage.usage_percent << "%\n";
    delete monitor;
#else
    std::cout << "NPU monitoring not supported on this platform.\n";
#endif
    return 0;
} 