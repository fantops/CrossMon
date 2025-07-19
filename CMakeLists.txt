cmake_minimum_required(VERSION 3.16)
project(CrossMon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Update include directories to add include/utils
include_directories(include/utils)

# Gather all source files, including utils
file(GLOB_RECURSE CROSSMON_SOURCES src/*.cpp src/utils/*.cpp)

# Main executable
add_executable(crossmon ${CROSSMON_SOURCES})
target_include_directories(crossmon PRIVATE include include/utils)

# Catch2 for unit testing
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(Catch2)

# Unit test target (add more test files as needed)
add_executable(cpu_monitor_mac_test test/test_cpu_monitor_mac.cpp src/macos/cpu_monitor_mac.cpp)
target_include_directories(cpu_monitor_mac_test PRIVATE include)
target_link_libraries(cpu_monitor_mac_test PRIVATE Catch2::Catch2WithMain)

# Link macOS frameworks if building on macOS
if(APPLE)
    target_link_libraries(crossmon PRIVATE "-framework CoreFoundation" "-framework IOKit")
    target_link_libraries(cpu_monitor_mac_test PRIVATE "-framework CoreFoundation" "-framework IOKit")
endif()

enable_testing()
add_test(NAME CpuMonitorMacTest COMMAND cpu_monitor_mac_test)

add_executable(test_statistics test/test_statistics.cpp src/utils/statistics.cpp)
target_include_directories(test_statistics PRIVATE include include/utils)
target_link_libraries(test_statistics PRIVATE Catch2::Catch2WithMain)
add_test(NAME StatisticsTest COMMAND test_statistics)

add_executable(test_monitor_args test/test_monitor_args.cpp src/utils/monitor_args.cpp)
target_include_directories(test_monitor_args PRIVATE include include/utils)
target_link_libraries(test_monitor_args PRIVATE Catch2::Catch2WithMain)
add_test(NAME MonitorArgsTest COMMAND test_monitor_args)

add_executable(test_process_manager test/test_process_manager.cpp src/utils/process_manager.cpp)
target_include_directories(test_process_manager PRIVATE include include/utils)
target_link_libraries(test_process_manager PRIVATE Catch2::Catch2WithMain)
add_test(NAME ProcessManagerTest COMMAND test_process_manager)
