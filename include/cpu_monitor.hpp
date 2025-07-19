#pragma once
#include <cstdint>
#include <chrono>

class ICpuMonitor {
public:
    virtual ~ICpuMonitor() = default;
    // Returns CPU busy percentage (0.0 - 100.0)
    virtual double getCpuBusy() = 0;
    // Returns true if this monitor is supported on the current platform
    virtual bool isSupported() const = 0;
}; 