#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../include/npu_monitor.hpp"
#include <memory>

#ifdef _WIN32
extern NPUMonitor* create_windows_npu_monitor();
#endif

TEST_CASE("Windows NPU Monitor Initialization and Usage", "[npu][windows]") {
#ifdef _WIN32
    std::unique_ptr<NPUMonitor> monitor(create_windows_npu_monitor());
    REQUIRE(monitor != nullptr);
    
    SECTION("Basic initialization and usage") {
        bool ok = monitor->initialize();
        NPUUsage usage = monitor->get_usage();
        
        if (ok) {
            REQUIRE(!usage.name.empty());
            REQUIRE(usage.usage_percent >= 0.0f);
            REQUIRE(usage.usage_percent <= 100.0f);
            INFO("NPU Name: " << usage.name << ", Usage: " << usage.usage_percent << "%");
        } else {
            INFO("NPU monitoring not available: " << usage.name);
            REQUIRE(!usage.name.empty()); // Should have explanation message
            REQUIRE(usage.usage_percent == 0.0f);
        }
    }
    
    SECTION("Multiple usage queries stability") {
        monitor->initialize();
        
        // Take multiple readings
        for (int i = 0; i < 5; ++i) {
            NPUUsage usage = monitor->get_usage();
            REQUIRE(usage.usage_percent >= 0.0f);
            REQUIRE(usage.usage_percent <= 100.0f);
            REQUIRE(!usage.name.empty());
        }
    }
    
    SECTION("Usage without initialization returns safe defaults") {
        // Don't call initialize()
        NPUUsage usage = monitor->get_usage();
        REQUIRE(usage.usage_percent == 0.0f);
        REQUIRE(!usage.name.empty());
    }
    
#else
    SUCCEED("NPU monitoring not supported on this platform.");
#endif
}

TEST_CASE("NPU Device Detection and Hardware Support", "[npu][windows]") {
#ifdef _WIN32
    std::unique_ptr<NPUMonitor> monitor(create_windows_npu_monitor());
    REQUIRE(monitor != nullptr);
    
    bool initialized = monitor->initialize();
    NPUUsage usage = monitor->get_usage();
    
    SECTION("Device detection provides meaningful information") {
        // Whether NPU is found or not, should provide useful info
        REQUIRE(!usage.name.empty());
        REQUIRE(usage.name.length() > 5); // Should be descriptive
        
        if (initialized) {
            INFO("NPU detected: " << usage.name);
            // If NPU is detected, usage should be retrievable
            REQUIRE(usage.usage_percent >= 0.0f);
        } else {
            INFO("NPU not detected: " << usage.name);
            // Should explain why NPU isn't available
            bool has_explanation = (usage.name.find("not") != std::string::npos ||
                                  usage.name.find("requires") != std::string::npos ||
                                  usage.name.find("detected") != std::string::npos);
            REQUIRE(has_explanation);
        }
    }
    
    SECTION("Performance monitoring consistency") {
        if (initialized) {
            std::vector<float> readings;
            for (int i = 0; i < 3; ++i) {
                NPUUsage usage = monitor->get_usage();
                readings.push_back(usage.usage_percent);
                
                // Each reading should be valid
                REQUIRE(std::isfinite(usage.usage_percent));
                REQUIRE(usage.usage_percent >= 0.0f);
                REQUIRE(usage.usage_percent <= 100.0f);
            }
            
            INFO("NPU usage readings: " << readings[0] << "%, " << readings[1] << "%, " << readings[2] << "%");
        }
    }
    
#else
    SUCCEED("NPU device detection not supported on this platform.");
#endif
}

TEST_CASE("NPU Memory Management and Resource Cleanup", "[npu][windows]") {
#ifdef _WIN32
    SECTION("Multiple monitor instances") {
        // Test creating and destroying multiple monitors
        for (int i = 0; i < 3; ++i) {
            std::unique_ptr<NPUMonitor> monitor(create_windows_npu_monitor());
            REQUIRE(monitor != nullptr);
            
            monitor->initialize();
            NPUUsage usage = monitor->get_usage();
            
            // Should work consistently across instances
            REQUIRE(!usage.name.empty());
            REQUIRE(usage.usage_percent >= 0.0f);
        }
        
        SUCCEED("Memory management test completed without crashes");
    }
    
    SECTION("Re-initialization behavior") {
        std::unique_ptr<NPUMonitor> monitor(create_windows_npu_monitor());
        
        bool result1 = monitor->initialize();
        bool result2 = monitor->initialize(); // Re-initialize
        
        // Should handle re-initialization gracefully
        NPUUsage usage = monitor->get_usage();
        REQUIRE(!usage.name.empty());
        REQUIRE(usage.usage_percent >= 0.0f);
    }
    
#else
    SUCCEED("NPU resource management tests not supported on this platform.");
#endif
} 