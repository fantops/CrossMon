#include "cpu_monitor.hpp"
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <vector>

#pragma comment(lib, "pdh.lib")

class WindowsCpuMonitor : public ICpuMonitor {
public:
    WindowsCpuMonitor() : hQuery(nullptr), hCounter(nullptr), initialized(false) {
        initialize();
    }

    ~WindowsCpuMonitor() {
        cleanup();
    }

    double getCpuBusy() override {
        if (!initialized) {
            return 0.0;
        }

        PDH_STATUS status = PdhCollectQueryData(hQuery);
        if (status != ERROR_SUCCESS) {
            return 0.0;
        }

        // Wait a bit for the counter to have data
        Sleep(100);

        status = PdhCollectQueryData(hQuery);
        if (status != ERROR_SUCCESS) {
            return 0.0;
        }

        PDH_FMT_COUNTERVALUE counterValue;
        status = PdhGetFormattedCounterValue(hCounter, PDH_FMT_DOUBLE, nullptr, &counterValue);
        if (status != ERROR_SUCCESS || counterValue.CStatus != PDH_CSTATUS_VALID_DATA) {
            return 0.0;
        }

        // Ensure the value is within expected range
        double cpuUsage = counterValue.doubleValue;
        if (cpuUsage < 0.0) cpuUsage = 0.0;
        if (cpuUsage > 100.0) cpuUsage = 100.0;

        return cpuUsage;
    }

    bool isSupported() const override {
        return initialized;
    }

private:
    PDH_HQUERY hQuery;
    PDH_HCOUNTER hCounter;
    bool initialized;

    void initialize() {
        PDH_STATUS status = PdhOpenQuery(nullptr, 0, &hQuery);
        if (status != ERROR_SUCCESS) {
            return;
        }

        // Add the processor time counter (total CPU usage)
        status = PdhAddEnglishCounterA(hQuery, "\\Processor(_Total)\\% Processor Time", 0, &hCounter);
        if (status != ERROR_SUCCESS) {
            cleanup();
            return;
        }

        // Collect initial data
        status = PdhCollectQueryData(hQuery);
        if (status != ERROR_SUCCESS) {
            cleanup();
            return;
        }

        initialized = true;
    }

    void cleanup() {
        if (hQuery) {
            PdhCloseQuery(hQuery);
            hQuery = nullptr;
        }
        hCounter = nullptr;
        initialized = false;
    }
};

// Factory function for use elsewhere
ICpuMonitor* createCpuMonitor() {
    return new WindowsCpuMonitor();
}
