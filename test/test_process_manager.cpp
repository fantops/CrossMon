#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "utils/process_manager.hpp"

TEST_CASE("isAppRunning: Finder should be running", "[process]") {
    REQUIRE(isAppRunning("Finder"));
}

TEST_CASE("isAppRunning: fake app should not be running", "[process]") {
    REQUIRE_FALSE(isAppRunning("DefinitelyNotARealApp"));
}

TEST_CASE("launchApp: launches app (manual check)", "[process]") {
    // This will open Calculator; user should see it open
    launchApp("Calculator");
    // Can't automatically check, but should not crash
    SUCCEED();
} 