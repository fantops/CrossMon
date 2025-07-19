#include "utils/monitor_args.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>

MonitorArgs parseMonitorArgs(int argc, char* argv[]) {
    MonitorArgs args;
    int argi = 1;
    while (argi < argc) {
        if (strcmp(argv[argi], "-i") == 0 && argi + 1 < argc) {
            args.interval = std::stoi(argv[argi + 1]);
            argi += 2;
        } else if (strcmp(argv[argi], "-o") == 0 && argi + 1 < argc) {
            args.resultPath = argv[argi + 1];
            argi += 2;
        } else {
            break;
        }
    }
    if (argi < argc) {
        args.appName = argv[argi];
    } // else leave appName empty
    return args;
} 