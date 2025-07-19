#pragma once
#include <string>

struct MonitorArgs {
    int interval = 1000;
    std::string resultPath;
    std::string appName; // May be empty if not provided
};

MonitorArgs parseMonitorArgs(int argc, char* argv[]); 