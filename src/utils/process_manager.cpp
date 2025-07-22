#include "utils/process_manager.hpp"
#include <cstdlib>
#include <string>
#include <algorithm>

#ifdef _WIN32
    #include <windows.h>
    #include <tlhelp32.h>
#endif

void launchApp(const std::string& appName) {
    if (appName.empty()) {
        return; // Don't try to launch empty app name
    }
    
#ifdef _WIN32
    // Windows: Use ShellExecute or system with "start"
    // Handle both .exe and app names
    std::string launchCmd;
    if (appName.find(".exe") != std::string::npos) {
        // If it's an .exe file, launch directly
        launchCmd = "start \"\" \"" + appName + "\"";
    } else {
        // If it's an app name, try to find it
        launchCmd = "start \"\" \"" + appName + "\"";
    }
    int result = system(launchCmd.c_str());
    (void)result; // Suppress unused variable warning
#else
    // macOS: Use open command
    std::string launchCmd = "open -a '" + appName + "'";
    int result = system(launchCmd.c_str());
    (void)result; // Suppress unused variable warning
#endif
}

bool isAppRunning(const std::string& appName) {
#ifdef _WIN32
    // Windows: Check running processes using ToolHelp32
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    bool found = false;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return false;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return false;
    }

    do {
        std::string processName(pe32.szExeFile);
        // Case-insensitive comparison for Windows
        std::transform(processName.begin(), processName.end(), processName.begin(), ::tolower);
        std::string searchName = appName;
        std::transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);
        
        if (processName == searchName) {
            found = true;
            break;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return found;
#else
    // macOS/Linux: Use pgrep
    std::string pgrepCmd = "pgrep -x '" + appName + "' > /dev/null";
    int ret = system(pgrepCmd.c_str());
    return ret == 0;
#endif
} 