#include "utils/output_formatter.hpp"
#include <iostream>
#include <fstream>

void printCpuStatsToConsole(const CpuStats& stats) {
    std::cout << "\n--- CPU Usage Statistics ---\n";
    std::cout << "Samples: " << stats.samples << std::endl;
    std::cout << "Peak:    " << stats.peak << "%" << std::endl;
    std::cout << "Average: " << stats.average << "%" << std::endl;
    std::cout << "Min:     " << stats.min << "%" << std::endl;
    std::cout << "Max:     " << stats.max << "%" << std::endl;
}

void writeCpuStatsToFile(const CpuStats& stats, const std::string& path) {
    std::ofstream out(path);
    if (out) {
        out << "Samples: " << stats.samples << "\n";
        out << "Peak:    " << stats.peak << "%\n";
        out << "Average: " << stats.average << "%\n";
        out << "Min:     " << stats.min << "%\n";
        out << "Max:     " << stats.max << "%\n";
        out.close();
        std::cout << "Results written to " << path << std::endl;
    } else {
        std::cerr << "Failed to write to " << path << std::endl;
    }
} 