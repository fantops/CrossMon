@echo off
REM CrossMon - Build and Test Script for Windows
REM This script builds CrossMon and runs basic tests

echo ========================================
echo CrossMon - Build and Test Script
echo ========================================
echo.

cd /d "%~dp0\.."

REM Create results directory if it doesn't exist
if not exist results mkdir results

set "LOG_FILE=results\build_and_test.log"

echo Logging output to: %LOG_FILE%
echo.

echo Step 1: Building CrossMon...
echo Step 1: Building CrossMon... >> %LOG_FILE%
echo.

REM Create build directory if it doesn't exist
if not exist build mkdir build

REM Configure with CMake
echo Configuring with CMake...
echo Configuring with CMake... >> %LOG_FILE%
cmake -B build -DCMAKE_BUILD_TYPE=Release >> %LOG_FILE% 2>&1

if %ERRORLEVEL% NEQ 0 (
    echo Error: CMake configuration failed!
    echo Error: CMake configuration failed! >> %LOG_FILE%
    pause
    exit /b 1
)

REM Build the project
echo Building project...
echo Building project... >> %LOG_FILE%
cmake --build build --config Release >> %LOG_FILE% 2>&1

if %ERRORLEVEL% NEQ 0 (
    echo Error: Build failed!
    echo Error: Build failed! >> %LOG_FILE%
    pause
    exit /b 1
)

echo.
echo Build completed successfully!
echo Build completed successfully! >> %LOG_FILE%
echo.

echo Step 2: Testing help functionality...
echo Step 2: Testing help functionality... >> %LOG_FILE%
echo.
build\Release\crossmon.exe --help >> %LOG_FILE% 2>&1

echo.
echo Step 3: Quick system test...
echo Step 3: Quick system test... >> %LOG_FILE%
echo.
call scripts\test_quick_windows.bat "%LOG_FILE%"

echo.
echo All tests completed!
echo All tests completed! >> %LOG_FILE%
echo Results saved to: %LOG_FILE%
pause
