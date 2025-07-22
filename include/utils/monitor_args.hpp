#pragma once
#include <string>

struct MonitorArgs {
    int interval = 1000;
    std::string resultPath;
    std::string appName; // May be empty if not provided
    bool showHelp = false;
    bool hasError = false;
    std::string errorMessage;
};

MonitorArgs parseMonitorArgs(int argc, char* argv[]);
void printHelp(const char* programName); 