#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "../include/npu_monitor.hpp"

TEST_CASE("Windows NPU Monitor Initialization and Usage") {
#ifdef _WIN32
    NPUMonitor* monitor = create_windows_npu_monitor();
    REQUIRE(monitor != nullptr);
    bool ok = monitor->initialize();
    NPUUsage usage = monitor->get_usage();
    if (ok) {
        REQUIRE(usage.name.find("NPU") != std::string::npos);
        REQUIRE(usage.usage_percent >= 0.0f);
        // Print for manual inspection
        INFO("NPU Name: " << usage.name << ", Usage: " << usage.usage_percent << "%");
    } else {
        INFO("NPU monitoring not available: " << usage.name);
    }
    delete monitor;
#else
    SUCCEED("NPU monitoring not supported on this platform.");
#endif
} 