#include "utils/output_formatter.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

void printCpuStatsToConsole(const CpuStats& stats) {
    std::cout << "\n--- CPU Usage Statistics ---\n";
    std::cout << "Samples: " << stats.samples << std::endl;
    std::cout << "Peak:    " << std::fixed << std::setprecision(1) << stats.peak << "%" << std::endl;
    std::cout << "Average: " << std::fixed << std::setprecision(1) << stats.average << "%" << std::endl;
    std::cout << "Min:     " << std::fixed << std::setprecision(1) << stats.min << "%" << std::endl;
    std::cout << "Max:     " << std::fixed << std::setprecision(1) << stats.max << "%" << std::endl;
}

void writeCpuStatsToFile(const CpuStats& stats, const std::string& path) {
    std::ofstream out(path);
    if (out) {
        out << "CPU Usage Statistics\n";
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

void printMemoryStatsToConsole(const MemoryStats& stats) {
    std::cout << "\n--- Memory Usage Statistics ---\n";
    std::cout << "Samples:     " << stats.samples << std::endl;
    std::cout << "Peak Used:   " << stats.peakUsedMB << " MB (" << std::fixed << std::setprecision(1) << stats.peakUsedPercent << "%)" << std::endl;
    std::cout << "Avg Used:    " << stats.avgUsedMB << " MB (" << std::fixed << std::setprecision(1) << stats.avgUsedPercent << "%)" << std::endl;
    std::cout << "Min Used:    " << stats.minUsedMB << " MB (" << std::fixed << std::setprecision(1) << stats.minUsedPercent << "%)" << std::endl;
    std::cout << "Max Used:    " << stats.maxUsedMB << " MB (" << std::fixed << std::setprecision(1) << stats.maxUsedPercent << "%)" << std::endl;
}

void writeMemoryStatsToFile(const MemoryStats& stats, const std::string& path) {
    std::ofstream out(path, std::ios::app); // Append mode
    if (out) {
        out << "\nMemory Usage Statistics\n";
        out << "Samples:     " << stats.samples << "\n";
        out << "Peak Used:   " << stats.peakUsedMB << " MB (" << stats.peakUsedPercent << "%)\n";
        out << "Avg Used:    " << stats.avgUsedMB << " MB (" << stats.avgUsedPercent << "%)\n";
        out << "Min Used:    " << stats.minUsedMB << " MB (" << stats.minUsedPercent << "%)\n";
        out << "Max Used:    " << stats.maxUsedMB << " MB (" << stats.maxUsedPercent << "%)\n";
        out.close();
    } else {
        std::cerr << "Failed to write memory stats to " << path << std::endl;
    }
}

void printGpuStatsToConsole(const GpuStats& stats) {
    std::cout << "\n--- GPU Usage Statistics ---\n";
    std::cout << "GPUs Found:      " << stats.gpuCount << std::endl;
    std::cout << "Samples:         " << stats.samples << std::endl;
    std::cout << "Peak GPU:        " << std::fixed << std::setprecision(1) << stats.peakUtilization << "%" << std::endl;
    std::cout << "Avg GPU:         " << std::fixed << std::setprecision(1) << stats.avgUtilization << "%" << std::endl;
    std::cout << "Min GPU:         " << std::fixed << std::setprecision(1) << stats.minUtilization << "%" << std::endl;
    std::cout << "Max GPU:         " << std::fixed << std::setprecision(1) << stats.maxUtilization << "%" << std::endl;
}

void writeGpuStatsToFile(const GpuStats& stats, const std::string& path) {
    std::ofstream out(path, std::ios::app); // Append mode
    if (out) {
        out << "\nGPU Usage Statistics\n";
        out << "GPUs Found:      " << stats.gpuCount << "\n";
        out << "Samples:         " << stats.samples << "\n";
        out << "Peak GPU:        " << stats.peakUtilization << "%\n";
        out << "Avg GPU:         " << stats.avgUtilization << "%\n";
        out << "Min GPU:         " << stats.minUtilization << "%\n";
        out << "Max GPU:         " << stats.maxUtilization << "%\n";
        out.close();
    } else {
        std::cerr << "Failed to write GPU stats to " << path << std::endl;
    }
}

void printSystemStatsToConsole(const SystemStats& stats) {
    printCpuStatsToConsole(stats.cpu);
    printMemoryStatsToConsole(stats.memory);
    printGpuStatsToConsole(stats.gpu);
}

void writeSystemStatsToFile(const SystemStats& stats, const std::string& path) {
    writeCpuStatsToFile(stats.cpu, path);
    writeMemoryStatsToFile(stats.memory, path);
    writeGpuStatsToFile(stats.gpu, path);
} 