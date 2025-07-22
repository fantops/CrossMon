@echo off
REM CrossMon - Quick System Monitor Test for Windows
REM This script runs CrossMon for 15 seconds to test basic functionality

REM Check if log file parameter was passed
set "PARENT_LOG=%~1"
if "%PARENT_LOG%"=="" (
    REM Create results directory if it doesn't exist
    if not exist results mkdir results
    
    set "LOG_FILE=results\test_quick.log"
) else (
    set "LOG_FILE=%PARENT_LOG%"
)

echo ========================================
echo CrossMon - Quick System Monitor Test
echo ========================================
echo ======================================== >> "%LOG_FILE%"
echo CrossMon - Quick System Monitor Test >> "%LOG_FILE%"
echo ======================================== >> "%LOG_FILE%"
echo.
echo This will monitor your system for 15 seconds
echo This will monitor your system for 15 seconds >> "%LOG_FILE%"
echo Press Ctrl+C to stop early if needed
echo Press Ctrl+C to stop early if needed >> "%LOG_FILE%"
echo.

cd /d "%~dp0\.."

REM Check if the executable exists
if not exist "build\Release\crossmon.exe" (
    echo Error: CrossMon executable not found!
    echo Error: CrossMon executable not found! >> "%LOG_FILE%"
    echo Please build the project first using:
    echo Please build the project first using: >> "%LOG_FILE%"
    echo   cmake --build build --config Release
    echo   cmake --build build --config Release >> "%LOG_FILE%"
    echo.
    if "%PARENT_LOG%"=="" pause
    exit /b 1
)

echo Starting CrossMon with 2-second intervals...
echo Starting CrossMon with 2-second intervals... >> "%LOG_FILE%"
echo.

REM Set output file in results directory
set "TEST_OUTPUT=results\crossmon_output.txt"

REM Run for about 15 seconds (7-8 samples)
timeout 1 > nul
start /min build\Release\crossmon.exe -i 2000

REM Wait for monitoring
timeout /t 15 /nobreak

REM Stop the process and capture output
taskkill /f /im crossmon.exe > nul 2>&1

echo.
echo Test completed!
echo Test completed! >> "%LOG_FILE%"
if "%PARENT_LOG%"=="" (
    echo Results saved to: %LOG_FILE%
    pause
)
