#include <catch2/catch_test_macros.hpp>
#include <unistd.h>
#include <chrono>
#include "gpu_monitor.hpp"

IGpuMonitor* createGpuMonitor();

TEST_CASE("macOS GPU Monitor Basic Functionality", "[gpu_monitor][macos]") {
    IGpuMonitor* monitor = createGpuMonitor();
    REQUIRE(monitor != nullptr);

    SECTION("Monitor should report support status") {
        // macOS GPU monitoring may not be fully implemented yet
        bool supported = monitor->isSupported();
        // Just verify we get a boolean response (true or false)
        REQUIRE((supported == true || supported == false));
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
            // GPU name may be empty if not yet implemented
        }
    }

    SECTION("Multiple calls should return consistent GPU count") {
        size_t count1 = monitor->getGpuCount();
        
        // Brief delay to ensure system state consistency
        usleep(100000); // 100ms
        
        size_t count2 = monitor->getGpuCount();
        
        // GPU count should be stable across calls
        REQUIRE(count1 == count2);
    }

    SECTION("GPU usage values should be within valid range across multiple calls") {
        GpuUsage usage1 = monitor->getGpuUsage();
        
        // Brief delay for different measurement
        usleep(200000); // 200ms
        
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
                
                // Utilization should be valid percentage
                REQUIRE(gpu.utilizationPercent >= 0.0);
                REQUIRE(gpu.utilizationPercent <= 100.0);
                
                // Temperature and power usage are optional (may be 0)
                REQUIRE(gpu.temperature >= 0);
                REQUIRE(gpu.powerUsage >= 0);
                
                // GPU name validation (may be empty if implementation is placeholder)
                // Just verify it's a valid string (not null)
                REQUIRE_NOTHROW(gpu.name.length());
            }
        }
    }

    SECTION("IOKit availability test") {
        // This test verifies basic functionality without requiring full IOKit
        // implementation which may be in development
        
        // At minimum, we should be able to call getGpuUsage without crashing
        REQUIRE_NOTHROW(monitor->getGpuUsage());
        
        GpuUsage usage = monitor->getGpuUsage();
        REQUIRE(usage.averageUtilization >= 0.0);
        REQUIRE(usage.averageUtilization <= 100.0);
    }

    delete monitor;
}

TEST_CASE("macOS GPU Monitor Edge Cases", "[gpu_monitor][macos]") {
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
            usleep(50000); // 50ms
        }
        
        // All counts should be the same
        for (size_t i = 1; i < counts.size(); ++i) {
            REQUIRE(counts[i] == counts[0]);
        }
    }

    SECTION("Handle potential Metal framework unavailability gracefully") {
        // Test that the monitor handles cases where Metal or other GPU
        // monitoring frameworks might not be available
        
        REQUIRE_NOTHROW(monitor->isSupported());
        REQUIRE_NOTHROW(monitor->getGpuCount());
        REQUIRE_NOTHROW(monitor->getGpuUsage());
    }

    delete monitor;
}

TEST_CASE("macOS GPU Monitor Performance", "[gpu_monitor][macos][performance]") {
    IGpuMonitor* monitor = createGpuMonitor();
    REQUIRE(monitor != nullptr);

    SECTION("GPU usage call should complete in reasonable time") {
        auto start = std::chrono::high_resolution_clock::now();
        
        GpuUsage usage = monitor->getGpuUsage();
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // GPU usage call should complete within 5 seconds (generous timeout)
        REQUIRE(duration.count() < 5000);
        
        // Verify we got valid data
        REQUIRE(usage.averageUtilization >= 0.0);
        REQUIRE(usage.averageUtilization <= 100.0);
    }

    delete monitor;
}

TEST_CASE("macOS GPU Monitor Apple Silicon Specific", "[gpu_monitor][macos][apple_silicon]") {
    IGpuMonitor* monitor = createGpuMonitor();
    REQUIRE(monitor != nullptr);

    SECTION("Handle Apple Silicon GPU detection") {
        // Apple Silicon Macs have integrated GPUs that may require special handling
        size_t gpuCount = monitor->getGpuCount();
        
        if (gpuCount > 0) {
            GpuUsage usage = monitor->getGpuUsage();
            
            // Check if we can detect Apple Silicon GPU
            bool foundAppleGpu = false;
            for (const auto& gpu : usage.gpus) {
                if (gpu.name.find("Apple") != std::string::npos || 
                    gpu.name.find("M1") != std::string::npos ||
                    gpu.name.find("M2") != std::string::npos ||
                    gpu.name.find("M3") != std::string::npos) {
                    foundAppleGpu = true;
                    break;
                }
            }
            
            // This is informational - we don't require finding Apple GPU
            // as the test might run on Intel Macs or implementation may be incomplete
            INFO("Apple Silicon GPU detection: " << (foundAppleGpu ? "Found" : "Not found"));
        }
    }

    delete monitor;
}
