#include "cpu_monitor.hpp"
#include <mach/mach.h>
#include <vector>
#include <numeric>

class MacCpuMonitor : public ICpuMonitor {
public:
    MacCpuMonitor() {
        lastCpuTicks = getCpuTicks();
    }

    double getCpuBusy() override {
        auto nowTicks = getCpuTicks();
        uint64_t totalDelta = 0, idleDelta = 0;
        for (size_t i = 0; i < nowTicks.size(); ++i) {
            totalDelta += (nowTicks[i] - lastCpuTicks[i]);
            if (i == CPU_STATE_IDLE) idleDelta += (nowTicks[i] - lastCpuTicks[i]);
        }
        lastCpuTicks = nowTicks;
        if (totalDelta == 0) return 0.0;
        return 100.0 * (totalDelta - idleDelta) / totalDelta;
    }

    bool isSupported() const override {
        return true; // Always supported on macOS
    }

private:
    std::vector<uint64_t> lastCpuTicks;

    std::vector<uint64_t> getCpuTicks() const {
        host_cpu_load_info_data_t cpuinfo;
        mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
        if (host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpuinfo, &count) != KERN_SUCCESS) {
            return {0, 0, 0, 0};
        }
        return {
            static_cast<uint64_t>(cpuinfo.cpu_ticks[CPU_STATE_USER]),
            static_cast<uint64_t>(cpuinfo.cpu_ticks[CPU_STATE_SYSTEM]),
            static_cast<uint64_t>(cpuinfo.cpu_ticks[CPU_STATE_IDLE]),
            static_cast<uint64_t>(cpuinfo.cpu_ticks[CPU_STATE_NICE])
        };
    }
};

// Factory function for use elsewhere
ICpuMonitor* createCpuMonitor() {
    return new MacCpuMonitor();
} 