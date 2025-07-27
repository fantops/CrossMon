#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "../include/npu_monitor.hpp"

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

TEST_CASE("Mac NPU Monitor always returns not supported") {
#ifdef __APPLE__
    MacNPUMonitor monitor;
    REQUIRE_FALSE(monitor.initialize());
    NPUUsage usage = monitor.get_usage();
    REQUIRE(usage.usage_percent == 0.0f);
    REQUIRE(usage.name.find("not supported") != std::string::npos);
#else
    SUCCEED("NPU monitoring not supported on this platform.");
#endif
} 