#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "utils/monitor_args.hpp"
#include <vector>
#include <string>

// Helper to create argc/argv
std::vector<char*> make_argv(const std::vector<std::string>& args) {
    std::vector<char*> argv;
    for (const auto& s : args) argv.push_back(const_cast<char*>(s.c_str()));
    argv.push_back(nullptr);
    return argv;
}

TEST_CASE("parseMonitorArgs: interval, output, app name", "[args]") {
    std::vector<std::string> args = {"prog", "-i", "200", "-o", "out.txt", "Safari"};
    auto argv = make_argv(args);
    MonitorArgs parsed = parseMonitorArgs(args.size(), argv.data());
    REQUIRE(parsed.interval == 200);
    REQUIRE(parsed.resultPath == "out.txt");
    REQUIRE(parsed.appName == "Safari");
}

TEST_CASE("parseMonitorArgs: only app name", "[args]") {
    std::vector<std::string> args = {"prog", "Safari"};
    auto argv = make_argv(args);
    MonitorArgs parsed = parseMonitorArgs(args.size(), argv.data());
    REQUIRE(parsed.interval == 1000);
    REQUIRE(parsed.resultPath == "");
    REQUIRE(parsed.appName == "Safari");
}

TEST_CASE("parseMonitorArgs: no app name provided", "[args]") {
    std::vector<std::string> args = {"prog", "-i", "100"};
    auto argv = make_argv(args);
    MonitorArgs parsed = parseMonitorArgs(args.size(), argv.data());
    REQUIRE(parsed.interval == 100);
    REQUIRE(parsed.resultPath == "");
    REQUIRE(parsed.appName == "");
} 