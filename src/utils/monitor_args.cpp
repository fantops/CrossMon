#include "utils/monitor_args.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>

MonitorArgs parseMonitorArgs(int argc, char* argv[]) {
    MonitorArgs args;
    int argi = 1;
    
    while (argi < argc) {
        if (strcmp(argv[argi], "-i") == 0 || strcmp(argv[argi], "--interval") == 0) {
            if (argi + 1 < argc) {
                args.interval = std::stoi(argv[argi + 1]);
                argi += 2;
            } else {
                args.hasError = true;
                args.errorMessage = "Error: -i/--interval requires a value (milliseconds)";
                return args;
            }
        } else if (strcmp(argv[argi], "-o") == 0 || strcmp(argv[argi], "--output") == 0) {
            if (argi + 1 < argc) {
                args.resultPath = argv[argi + 1];
                argi += 2;
            } else {
                args.hasError = true;
                args.errorMessage = "Error: -o/--output requires a file path";
                return args;
            }
        } else if (strcmp(argv[argi], "-s") == 0 || strcmp(argv[argi], "--samples") == 0) {
            // Skip samples argument for now (not implemented in current version)
            if (argi + 1 < argc) {
                argi += 2;
            } else {
                args.hasError = true;
                args.errorMessage = "Error: -s/--samples requires a value";
                return args;
            }
        } else if (strcmp(argv[argi], "-h") == 0 || strcmp(argv[argi], "--help") == 0) {
            args.showHelp = true;
            return args;
        } else if (argv[argi][0] == '-') {
            args.hasError = true;
            args.errorMessage = std::string("Error: Unknown option ") + argv[argi];
            return args;
        } else {
            // This is the application name
            args.appName = argv[argi];
            argi++;
            break;
        }
    }
    
    return args;
}

void printHelp(const char* programName) {
    std::cout << "CrossMon - Cross-platform System Resource Monitor\n\n";
    std::cout << "Usage: " << programName << " [OPTIONS] [APPLICATION_NAME]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help                Show this help message\n";
    std::cout << "  -i, --interval MSEC       Monitoring interval in milliseconds (default: 1000)\n";
    std::cout << "  -o, --output FILE         Output statistics to file\n\n";
    std::cout << "Arguments:\n";
    std::cout << "  APPLICATION_NAME          Monitor system while this application is running\n";
    std::cout << "                           If not provided, monitor until Ctrl+C is pressed\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << programName << " --help\n";
    std::cout << "  " << programName << " -i 500 notepad.exe\n";
    std::cout << "  " << programName << " --interval 2000 --output stats.txt chrome.exe\n";
    std::cout << "  " << programName << " -o system_stats.txt\n\n";
    std::cout << "The program monitors CPU, Memory, and GPU usage in real-time.\n";
    std::cout << "Press Ctrl+C to stop monitoring and view statistics.\n";
} 