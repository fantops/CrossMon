cmake_minimum_required(VERSION 3.16)
project(CrossMon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Update include directories to add include/utils
include_directories(include/utils)

# Gather source files based on platform
file(GLOB_RECURSE UTILS_SOURCES src/utils/*.cpp)
set(COMMON_SOURCES src/main.cpp ${UTILS_SOURCES})

# Platform-specific sources
if(WIN32)
    file(GLOB PLATFORM_SOURCES src/windows/*.cpp)
    set(CROSSMON_SOURCES ${COMMON_SOURCES} ${PLATFORM_SOURCES} src/windows/npu_monitor_win.cpp)
elseif(APPLE)
    file(GLOB PLATFORM_SOURCES src/macos/*.cpp)
    set(CROSSMON_SOURCES ${COMMON_SOURCES} ${PLATFORM_SOURCES} src/macos/npu_monitor_mac.cpp)
else()
    # Linux or other Unix-like systems (future implementation)
    message(FATAL_ERROR "Unsupported platform. Currently supports Windows and macOS only.")
endif()

# Main executable
add_executable(crossmon ${CROSSMON_SOURCES})
target_include_directories(crossmon PRIVATE include include/utils)

# Platform-specific linking
if(APPLE)
    target_link_libraries(crossmon PRIVATE "-framework CoreFoundation" "-framework IOKit")
elseif(WIN32)
    target_link_libraries(crossmon PRIVATE pdh kernel32 psapi dxgi d3d11 wbemuuid ole32 oleaut32)
endif()

# Catch2 for unit testing
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(Catch2)

# Enable testing
enable_testing()

# Platform-specific unit tests
if(APPLE)
    add_executable(cpu_monitor_mac_test test/test_cpu_monitor_mac.cpp src/macos/cpu_monitor_mac.cpp)
    target_include_directories(cpu_monitor_mac_test PRIVATE include)
    target_link_libraries(cpu_monitor_mac_test PRIVATE Catch2::Catch2WithMain "-framework CoreFoundation" "-framework IOKit")
    add_test(NAME CpuMonitorMacTest COMMAND cpu_monitor_mac_test)

    add_executable(gpu_monitor_mac_test test/test_gpu_monitor_mac.cpp src/macos/gpu_monitor_mac.cpp)
    target_include_directories(gpu_monitor_mac_test PRIVATE include)
    target_link_libraries(gpu_monitor_mac_test PRIVATE Catch2::Catch2WithMain "-framework CoreFoundation" "-framework IOKit")
    add_test(NAME GpuMonitorMacTest COMMAND gpu_monitor_mac_test)

    add_executable(test_npu_monitor_mac test/test_npu_monitor_mac.cpp src/macos/npu_monitor_mac.cpp)
    target_include_directories(test_npu_monitor_mac PRIVATE include)
    target_link_libraries(test_npu_monitor_mac PRIVATE Catch2::Catch2WithMain)
    add_test(NAME NPUMonitorMacTest COMMAND test_npu_monitor_mac)
elseif(WIN32)
    add_executable(cpu_monitor_win_test test/test_cpu_monitor_win.cpp src/windows/cpu_monitor_win.cpp)
    target_include_directories(cpu_monitor_win_test PRIVATE include)
    target_link_libraries(cpu_monitor_win_test PRIVATE Catch2::Catch2WithMain pdh kernel32)
    add_test(NAME CpuMonitorWinTest COMMAND cpu_monitor_win_test)

    add_executable(gpu_monitor_win_test test/test_gpu_monitor_win.cpp src/windows/gpu_monitor_win.cpp)
    target_include_directories(gpu_monitor_win_test PRIVATE include)
    target_link_libraries(gpu_monitor_win_test PRIVATE Catch2::Catch2WithMain wbemuuid ole32 oleaut32 dxgi d3d11)
    add_test(NAME GpuMonitorWinTest COMMAND gpu_monitor_win_test)
    add_executable(test_npu_monitor_win test/test_npu_monitor_win.cpp src/windows/npu_monitor_win.cpp)
    target_include_directories(test_npu_monitor_win PRIVATE include)
    target_link_libraries(test_npu_monitor_win PRIVATE Catch2::Catch2WithMain)
    add_test(NAME NPUMonitorWinTest COMMAND test_npu_monitor_win)
endif()

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
if(WIN32)
    target_link_libraries(test_process_manager PRIVATE kernel32)
endif()
add_test(NAME ProcessManagerTest COMMAND test_process_manager)
