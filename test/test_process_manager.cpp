#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "utils/process_manager.hpp"

TEST_CASE("isAppRunning: system process should be running", "[process]") {
#ifdef _WIN32
    // Windows Explorer should always be running
    REQUIRE(isAppRunning("explorer.exe"));
#else
    // macOS Finder should always be running
    REQUIRE(isAppRunning("Finder"));
#endif
}

TEST_CASE("isAppRunning: fake app should not be running", "[process]") {
    REQUIRE_FALSE(isAppRunning("DefinitelyNotARealApp"));
}

TEST_CASE("launchApp: launches app (manual check)", "[process]") {
#ifdef _WIN32
    // This will open Calculator on Windows
    launchApp("calc.exe");
#else
    // This will open Calculator on macOS
    launchApp("Calculator");
#endif
    // Can't automatically check, but should not crash
    SUCCEED();
} 