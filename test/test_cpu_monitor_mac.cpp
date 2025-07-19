#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "cpu_monitor.hpp"
#include <thread>

extern ICpuMonitor* createCpuMonitor();

TEST_CASE("MacCpuMonitor basic functionality", "[macos][cpu]") {
    ICpuMonitor* monitor = createCpuMonitor();
    REQUIRE(monitor->isSupported());
    // Take two samples to allow delta calculation
    monitor->getCpuBusy();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    double busy = monitor->getCpuBusy();
    // CPU busy should be between 0 and 100
    REQUIRE(busy >= 0.0);
    REQUIRE(busy <= 100.0);
    delete monitor;
} 