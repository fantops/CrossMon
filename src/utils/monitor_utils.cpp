#include "utils/monitor_utils.hpp"
#include "utils/process_manager.hpp"
#include "utils/statistics.hpp"
#include "utils/output_formatter.hpp"
#include "cpu_monitor.hpp"
#include "memory_monitor.hpp"
#include "gpu_monitor.hpp"
#include <thread>
#include <chrono>
#include <vector>
#include <iostream>
#include <csignal>
#include <atomic>
#include <iomanip>

ICpuMonitor* createCpuMonitor();
IMemoryMonitor* createMemoryMonitor();
IGpuMonitor* createGpuMonitor();

namespace {
std::atomic<bool> keepRunning(true);
void signalHandler(int) { keepRunning = false; }
}

void monitorSystemUsage(const MonitorArgs& args, SystemSamples& samples) {
    std::signal(SIGINT, signalHandler);
    
    ICpuMonitor* cpuMonitor = createCpuMonitor();
    IMemoryMonitor* memoryMonitor = createMemoryMonitor();
    IGpuMonitor* gpuMonitor = createGpuMonitor();
    
    // Initialize CPU monitor and trigger GPU detection
    cpuMonitor->getCpuBusy();
    
    // Trigger GPU enumeration to get accurate count
    GpuUsage initialGpuCheck = gpuMonitor->getGpuUsage();
    samples.gpuCount = gpuMonitor->getGpuCount();
    
    std::cout << "System Information:\n";
    std::cout << "GPUs detected: " << samples.gpuCount << std::endl;
    
    if (!args.appName.empty()) {
        std::cout << "Monitoring system usage while " << args.appName << " is running...\n";
        std::cout << "Press Ctrl+C to stop and see statistics.\n\n";
        
        while (keepRunning && isAppRunning(args.appName)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(args.interval));
            
            // Get CPU usage
            double cpu = cpuMonitor->getCpuBusy();
            samples.cpuUsage.push_back(cpu);
            
            // Get memory usage
            MemoryUsage mem = memoryMonitor->getMemoryUsage();
            samples.memoryUsedMB.push_back(mem.usedPhysicalMB);
            samples.memoryUsedPercent.push_back(mem.usedPercentage);
            
            // Get GPU usage
            GpuUsage gpu = gpuMonitor->getGpuUsage();
            samples.gpuUtilization.push_back(gpu.averageUtilization);
            
            // Display current usage with individual GPU info
            std::cout << "CPU: " << std::fixed << std::setprecision(1) << cpu << "% | "
                      << "Memory: " << mem.usedPhysicalMB << " MB ("
                      << std::fixed << std::setprecision(1) << mem.usedPercentage << "%) | ";
            
            if (gpu.gpus.size() > 1) {
                std::cout << "GPUs: ";
                for (size_t i = 0; i < gpu.gpus.size(); ++i) {
                    if (i > 0) std::cout << ", ";
                    std::cout << std::fixed << std::setprecision(1) << gpu.gpus[i].utilizationPercent << "%";
                }
                std::cout << " (avg: " << std::fixed << std::setprecision(1) << gpu.averageUtilization << "%)";
            } else {
                std::cout << "GPU: " << std::fixed << std::setprecision(1) << gpu.averageUtilization << "%";
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "Monitoring system usage...\n";
        std::cout << "Press Ctrl+C to stop and see statistics.\n\n";
        
        while (keepRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(args.interval));
            
            // Get CPU usage
            double cpu = cpuMonitor->getCpuBusy();
            samples.cpuUsage.push_back(cpu);
            
            // Get memory usage
            MemoryUsage mem = memoryMonitor->getMemoryUsage();
            samples.memoryUsedMB.push_back(mem.usedPhysicalMB);
            samples.memoryUsedPercent.push_back(mem.usedPercentage);
            
            // Get GPU usage
            GpuUsage gpu = gpuMonitor->getGpuUsage();
            samples.gpuUtilization.push_back(gpu.averageUtilization);
            
            // Display current usage
            std::cout << "CPU: " << std::fixed << std::setprecision(1) << cpu << "% | "
                      << "Memory: " << mem.usedPhysicalMB << " MB ("
                      << std::fixed << std::setprecision(1) << mem.usedPercentage << "%) | ";
            
            // GPU display - individual GPUs if multiple detected
            if (gpu.gpus.size() > 1) {
                std::cout << "GPUs: ";
                for (size_t i = 0; i < gpu.gpus.size(); ++i) {
                    if (i > 0) std::cout << ", ";
                    std::cout << std::fixed << std::setprecision(1) << gpu.gpus[i].utilizationPercent << "%";
                }
                std::cout << " (avg: " << std::fixed << std::setprecision(1) << gpu.averageUtilization << "%)";
            } else {
                std::cout << "GPU: " << std::fixed << std::setprecision(1) << gpu.averageUtilization << "%";
            }
            
            std::cout << std::endl;
        }
    }
    
    delete cpuMonitor;
    delete memoryMonitor;
    delete gpuMonitor;
}

void outputSystemStatistics(const SystemSamples& samples, const std::string& resultPath) {
    SystemStats stats;
    stats.cpu = computeCpuStats(samples.cpuUsage);
    stats.memory = computeMemoryStats(samples.memoryUsedMB, samples.memoryUsedPercent);
    stats.gpu = computeGpuStats(samples.gpuUtilization, samples.gpuCount);
    
    printSystemStatsToConsole(stats);
    
    if (!resultPath.empty()) {
        writeSystemStatsToFile(stats, resultPath);
    }
}

// Legacy functions for backward compatibility
void monitorCpuUsage(const MonitorArgs& args, std::vector<double>& samples) {
    std::signal(SIGINT, signalHandler);
    ICpuMonitor* monitor = createCpuMonitor();
    monitor->getCpuBusy(); // Initialize
    if (!args.appName.empty()) {
        while (keepRunning && isAppRunning(args.appName)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(args.interval));
            double cpu = monitor->getCpuBusy();
            samples.push_back(cpu);
        }
    } else {
        while (keepRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(args.interval));
            double cpu = monitor->getCpuBusy();
            samples.push_back(cpu);
        }
    }
    delete monitor;
}

void outputStatistics(const std::vector<double>& samples, const std::string& resultPath) {
    CpuStats stats = computeCpuStats(samples);
    printCpuStatsToConsole(stats);
    if (!resultPath.empty()) {
        writeCpuStatsToFile(stats, resultPath);
    }
} 