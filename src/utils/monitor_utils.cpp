#include "utils/monitor_utils.hpp"
#include "utils/process_manager.hpp"
#include "utils/statistics.hpp"
#include "utils/output_formatter.hpp"
#include "cpu_monitor.hpp"
#include "memory_monitor.hpp"
#include "gpu_monitor.hpp"
#ifdef _WIN32
#include "npu_monitor.hpp"
#endif
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
    
#ifdef _WIN32
    NPUMonitor* npuMonitor = create_windows_npu_monitor();
    bool npuAvailable = npuMonitor->initialize();
    if (npuAvailable) {
        NPUUsage npuInfo = npuMonitor->get_usage();
        samples.npuName = npuInfo.name;
    }
#else
    bool npuAvailable = false;
#endif
    
    // Initialize CPU monitor and trigger GPU detection
    cpuMonitor->getCpuBusy();
    
    // Get GPU count
    GpuUsage initialGpuCheck = gpuMonitor->getGpuUsage();
    samples.gpuCount = gpuMonitor->getGpuCount();
    
#ifdef _DEBUG
    std::cout << "[DEBUG] GPU initialization complete. Count: " << samples.gpuCount << std::endl;
    std::cout << "[DEBUG] Initial GPU check - Average utilization: " 
              << initialGpuCheck.averageUtilization << "%" << std::endl;
    for (size_t i = 0; i < initialGpuCheck.gpus.size(); ++i) {
        std::cout << "[DEBUG] GPU " << i << ": " << initialGpuCheck.gpus[i].name 
                  << " - " << initialGpuCheck.gpus[i].utilizationPercent << "%" << std::endl;
    }
#endif
    
    std::cout << "System Information:\n";
    std::cout << "GPUs detected: " << samples.gpuCount << std::endl;
#ifdef _WIN32
    if (npuAvailable) {
        std::cout << "NPU detected: " << samples.npuName << std::endl;
    } else {
        std::cout << "NPU: Not available" << std::endl;
    }
#else
    std::cout << "NPU: Not supported on this platform" << std::endl;
#endif
    
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
            
            // Get NPU usage
#ifdef _WIN32
            double npuUsage = 0.0;
            if (npuAvailable) {
                NPUUsage npu = npuMonitor->get_usage();
                npuUsage = npu.usage_percent;
            }
            samples.npuUtilization.push_back(npuUsage);
#endif
            
            // Display current usage with individual GPU info
            std::cout << "CPU: " << std::fixed << std::setprecision(1) << cpu << "% | "
                      << "Memory: " << mem.usedPhysicalMB << " MB ("
                      << std::fixed << std::setprecision(1) << mem.usedPercentage << "%) | ";
            
#ifdef _DEBUG
            std::cout << "[DEBUG: " << gpu.gpus.size() << " GPUs] ";
#endif
            if (gpu.gpus.size() > 1) {
                std::cout << "GPUs: ";
                for (size_t i = 0; i < gpu.gpus.size(); ++i) {
                    if (i > 0) std::cout << ", ";
                    std::cout << std::fixed << std::setprecision(1) << gpu.gpus[i].utilizationPercent << "%";
#ifdef _DEBUG
                    std::cout << "(" << gpu.gpus[i].name << ")";
#endif
                }
                std::cout << " (avg: " << std::fixed << std::setprecision(1) << gpu.averageUtilization << "%)";
            } else {
                std::cout << "GPU: " << std::fixed << std::setprecision(1) << gpu.averageUtilization << "%";
            }
            
#ifdef _WIN32
            if (npuAvailable) {
                std::cout << " | NPU: " << std::fixed << std::setprecision(1) << npuUsage << "%";
            }
#endif
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
            
            // Get NPU usage
#ifdef _WIN32
            double npuUsage = 0.0;
            if (npuAvailable) {
                NPUUsage npu = npuMonitor->get_usage();
                npuUsage = npu.usage_percent;
            }
            samples.npuUtilization.push_back(npuUsage);
#endif
            
            // Display current usage
            std::cout << "CPU: " << std::fixed << std::setprecision(1) << cpu << "% | "
                      << "Memory: " << mem.usedPhysicalMB << " MB ("
                      << std::fixed << std::setprecision(1) << mem.usedPercentage << "%) | ";
            
#ifdef _DEBUG
            std::cout << "[DEBUG: " << gpu.gpus.size() << " GPUs] ";
#endif
            // GPU display - individual GPUs if multiple detected
            if (gpu.gpus.size() > 1) {
                std::cout << "GPUs: ";
                for (size_t i = 0; i < gpu.gpus.size(); ++i) {
                    if (i > 0) std::cout << ", ";
                    std::cout << std::fixed << std::setprecision(1) << gpu.gpus[i].utilizationPercent << "%";
#ifdef _DEBUG
                    std::cout << "(" << gpu.gpus[i].name << ")";
#endif
                }
                std::cout << " (avg: " << std::fixed << std::setprecision(1) << gpu.averageUtilization << "%)";
            } else {
                std::cout << "GPU: " << std::fixed << std::setprecision(1) << gpu.averageUtilization << "%";
            }
            
#ifdef _WIN32
            if (npuAvailable) {
                std::cout << " | NPU: " << std::fixed << std::setprecision(1) << npuUsage << "%";
            }
#endif
            
            std::cout << std::endl;
        }
    }
    
    delete cpuMonitor;
    delete memoryMonitor;
    delete gpuMonitor;
#ifdef _WIN32
    delete npuMonitor;
#endif
}

void outputSystemStatistics(const SystemSamples& samples, const std::string& resultPath) {
    SystemStats stats;
    stats.cpu = computeCpuStats(samples.cpuUsage);
    stats.memory = computeMemoryStats(samples.memoryUsedMB, samples.memoryUsedPercent);
    stats.gpu = computeGpuStats(samples.gpuUtilization, samples.gpuCount);
#ifdef _WIN32
    stats.npu = computeNpuStats(samples.npuUtilization, samples.npuName);
#endif
    
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