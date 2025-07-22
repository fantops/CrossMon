@echo off
REM CrossMon - GPU Detection Test for Windows
REM This script specifically tests GPU detection and enumeration

REM Create results directory if it doesn't exist
if not exist results mkdir results

set "LOG_FILE=results\gpu_detection.log"

echo ========================================
echo CrossMon - GPU Detection Test
echo ========================================
echo ======================================== >> "%LOG_FILE%"
echo CrossMon - GPU Detection Test >> "%LOG_FILE%"
echo ======================================== >> "%LOG_FILE%"
echo.
echo This test will check if CrossMon can detect your GPUs correctly
echo This test will check if CrossMon can detect your GPUs correctly >> "%LOG_FILE%"
echo.
echo Logging output to: %LOG_FILE%
echo.

cd /d "%~dp0\.."

REM Check if the executable exists
if not exist "build\Release\crossmon.exe" (
    echo Error: CrossMon executable not found!
    echo Error: CrossMon executable not found! >> "%LOG_FILE%"
    echo Please build the project first.
    echo Please build the project first. >> "%LOG_FILE%"
    pause
    exit /b 1
)

echo Testing GPU detection...
echo Testing GPU detection... >> "%LOG_FILE%"
echo.

REM Set output file in results directory
set "GPU_OUTPUT=results\gpu_detection_output.txt"

REM Start the application briefly to capture GPU detection
start /min cmd /c "build\Release\crossmon.exe -i 5000 > %GPU_OUTPUT% 2>&1"

REM Wait for initialization and one monitoring cycle
timeout /t 6 /nobreak > nul

REM Kill the application
taskkill /f /im crossmon.exe > nul 2>&1

echo === GPU Detection Results ===
echo === GPU Detection Results === >> "%LOG_FILE%"
type "%GPU_OUTPUT%"
type "%GPU_OUTPUT%" >> "%LOG_FILE%"

echo.
echo Compare this with Windows Task Manager ^> Performance ^> GPU
echo Compare this with Windows Task Manager ^> Performance ^> GPU >> "%LOG_FILE%"
echo to verify all your GPUs are detected.
echo to verify all your GPUs are detected. >> "%LOG_FILE%"
echo.
echo Test completed! Results saved to:
echo - Log file: %LOG_FILE%
echo - GPU output: %GPU_OUTPUT%
pause
