#include <catch2/catch_test_macros.hpp>
#include <windows.h>
#include "cpu_monitor.hpp"

ICpuMonitor* createCpuMonitor();

TEST_CASE("Windows CPU Monitor Basic Functionality", "[cpu_monitor]") {
    ICpuMonitor* monitor = createCpuMonitor();
    REQUIRE(monitor != nullptr);

    SECTION("Monitor should be supported on Windows") {
        REQUIRE(monitor->isSupported() == true);
    }

    SECTION("CPU usage should be within valid range") {
        double cpuUsage = monitor->getCpuBusy();
        REQUIRE(cpuUsage >= 0.0);
        REQUIRE(cpuUsage <= 100.0);
    }

    SECTION("Multiple calls should return reasonable values") {
        double usage1 = monitor->getCpuBusy();
        
        // Sleep briefly to get different measurement
        Sleep(200);
        
        double usage2 = monitor->getCpuBusy();
        
        REQUIRE(usage1 >= 0.0);
        REQUIRE(usage1 <= 100.0);
        REQUIRE(usage2 >= 0.0);
        REQUIRE(usage2 <= 100.0);
    }

    delete monitor;
}
