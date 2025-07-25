#include <catch2/catch_test_macros.hpp>
#include <windows.h>
#include "gpu_monitor.hpp"

IGpuMonitor* createGpuMonitor();

TEST_CASE("Windows GPU Monitor Basic Functionality", "[gpu_monitor]") {
    IGpuMonitor* monitor = createGpuMonitor();
    REQUIRE(monitor != nullptr);

    SECTION("Monitor should be supported on Windows") {
        REQUIRE(monitor->isSupported() == true);
    }

    SECTION("GPU count should be reasonable") {
        size_t gpuCount = monitor->getGpuCount();
        REQUIRE(gpuCount >= 0);
        REQUIRE(gpuCount <= 16); // Reasonable upper limit for system GPUs
    }

    SECTION("GPU usage should return valid structure") {
        GpuUsage gpuUsage = monitor->getGpuUsage();
        
        // Check that we get a valid structure
        REQUIRE(gpuUsage.gpus.size() >= 0);
        REQUIRE(gpuUsage.averageUtilization >= 0.0);
        REQUIRE(gpuUsage.averageUtilization <= 100.0);
        
        // If GPUs are detected, validate each GPU info
        for (const auto& gpu : gpuUsage.gpus) {
            REQUIRE(gpu.utilizationPercent >= 0.0);
            REQUIRE(gpu.utilizationPercent <= 100.0);
            REQUIRE_FALSE(gpu.name.empty()); // GPU should have a name
        }
    }

    SECTION("Multiple calls should return consistent GPU count") {
        size_t count1 = monitor->getGpuCount();
        
        // Brief delay to ensure system state consistency
        Sleep(50); // Reduced from 100ms
        
        size_t count2 = monitor->getGpuCount();
        
        // GPU count should be stable across calls
        REQUIRE(count1 == count2);
    }

    SECTION("GPU usage values should be within valid range across multiple calls") {
        GpuUsage usage1 = monitor->getGpuUsage();
        
        // Brief delay for different measurement
        Sleep(100); // Reduced from 200ms
        
        GpuUsage usage2 = monitor->getGpuUsage();
        
        // Validate first measurement
        REQUIRE(usage1.averageUtilization >= 0.0);
        REQUIRE(usage1.averageUtilization <= 100.0);
        
        // Validate second measurement
        REQUIRE(usage2.averageUtilization >= 0.0);
        REQUIRE(usage2.averageUtilization <= 100.0);
        
        // Both should have same number of GPUs
        REQUIRE(usage1.gpus.size() == usage2.gpus.size());
    }

    SECTION("GPU info consistency") {
        if (monitor->getGpuCount() > 0) {
            GpuUsage usage = monitor->getGpuUsage();
            
            // Number of GPU infos should match reported count
            REQUIRE(usage.gpus.size() == monitor->getGpuCount());
            
            // Each GPU should have valid information
            for (size_t i = 0; i < usage.gpus.size(); ++i) {
                const auto& gpu = usage.gpus[i];
                
                // GPU name should not be empty
                REQUIRE_FALSE(gpu.name.empty());
                
                // Utilization should be valid percentage
                REQUIRE(gpu.utilizationPercent >= 0.0);
                REQUIRE(gpu.utilizationPercent <= 100.0);
                
                // Temperature and power usage are optional (may be 0)
                REQUIRE(gpu.temperature >= 0);
                REQUIRE(gpu.powerUsage >= 0);
            }
        }
    }

    SECTION("WMI availability test") {
        // This test verifies that WMI services are available
        // which is required for Windows GPU monitoring
        
        HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
        bool comInitialized = (hr == S_OK || hr == S_FALSE);
        
        if (comInitialized) {
            // If COM is available, GPU monitoring should work
            REQUIRE(monitor->isSupported() == true);
            
            // At minimum, we should be able to call getGpuUsage without crashing
            GpuUsage usage = monitor->getGpuUsage();
            REQUIRE(usage.averageUtilization >= 0.0);
            
            CoUninitialize();
        }
    }

    delete monitor;
}

TEST_CASE("Windows GPU Monitor Edge Cases", "[gpu_monitor]") {
    IGpuMonitor* monitor = createGpuMonitor();
    REQUIRE(monitor != nullptr);

    SECTION("Rapid successive calls should not crash") {
        // Test rapid calls to ensure thread safety and stability
        for (int i = 0; i < 10; ++i) {
            GpuUsage usage = monitor->getGpuUsage();
            REQUIRE(usage.averageUtilization >= 0.0);
            REQUIRE(usage.averageUtilization <= 100.0);
        }
    }

    SECTION("GPU count should be stable") {
        // GPU count should not change during normal operation
        std::vector<size_t> counts;
        for (int i = 0; i < 5; ++i) {
            counts.push_back(monitor->getGpuCount());
            Sleep(25); // Reduced from 50ms
        }
        
        // All counts should be the same
        for (size_t i = 1; i < counts.size(); ++i) {
            REQUIRE(counts[i] == counts[0]);
        }
    }

    delete monitor;
}

TEST_CASE("Windows GPU Monitor Performance", "[gpu_monitor][performance]") {
    IGpuMonitor* monitor = createGpuMonitor();
    REQUIRE(monitor != nullptr);

    SECTION("GPU usage call should complete in reasonable time") {
        auto start = GetTickCount64();
        
        GpuUsage usage = monitor->getGpuUsage();
        
        auto duration = GetTickCount64() - start;
        
        // GPU usage call should complete within 5 seconds (generous timeout)
        REQUIRE(duration < 5000);
        
        // Verify we got valid data
        REQUIRE(usage.averageUtilization >= 0.0);
        REQUIRE(usage.averageUtilization <= 100.0);
    }

    delete monitor;
}
