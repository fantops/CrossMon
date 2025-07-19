#include "utils/process_manager.hpp"
#include <cstdlib>

void launchApp(const std::string& appName) {
    std::string launchCmd = "open -a '" + appName + "'";
    system(launchCmd.c_str());
}

bool isAppRunning(const std::string& appName) {
    std::string pgrepCmd = "pgrep -x '" + appName + "' > /dev/null";
    int ret = system(pgrepCmd.c_str());
    return ret == 0;
} 