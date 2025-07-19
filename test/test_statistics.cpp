#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "utils/statistics.hpp"
#include <vector>

TEST_CASE("Statistics: average, peak, min, max", "[statistics]") {
    std::vector<double> values = {10.0, 20.0, 30.0, 40.0, 50.0};
    CpuStats stats = computeCpuStats(values);
    REQUIRE(stats.samples == 5);
    REQUIRE(stats.peak == 50.0);
    REQUIRE(stats.max == 50.0);
    REQUIRE(stats.min == 10.0);
    REQUIRE(stats.average == Catch::Approx(30.0));
}

TEST_CASE("Statistics: empty vector", "[statistics]") {
    std::vector<double> values;
    CpuStats stats = computeCpuStats(values);
    REQUIRE(stats.samples == 0);
    REQUIRE(stats.peak == 0.0);
    REQUIRE(stats.max == 0.0);
    REQUIRE(stats.min == 0.0);
    REQUIRE(stats.average == 0.0);
} 