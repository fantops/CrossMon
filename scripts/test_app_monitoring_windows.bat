@echo off
REM CrossMon - Application Monitoring Test for Windows
REM This script tests monitoring while a specific application runs

REM Create results directory if it doesn't exist
if not exist results mkdir results

set "LOG_FILE=results\app_monitoring.log"

echo ========================================
echo CrossMon - Application Monitoring Test
echo ========================================
echo ======================================== >> "%LOG_FILE%"
echo CrossMon - Application Monitoring Test >> "%LOG_FILE%"
echo ======================================== >> "%LOG_FILE%"
echo.
echo This test will monitor system usage while Notepad is running
echo This test will monitor system usage while Notepad is running >> "%LOG_FILE%"
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

echo Starting Notepad and monitoring system usage...
echo Starting Notepad and monitoring system usage... >> "%LOG_FILE%"
echo Close Notepad to stop monitoring or press Ctrl+C
echo Close Notepad to stop monitoring or press Ctrl+C >> "%LOG_FILE%"
echo.

REM Set output file in results directory
set "APP_OUTPUT=results\notepad_monitoring.txt"

REM Run CrossMon to monitor while notepad is running
echo Monitoring will start in 2 seconds...
echo Monitoring will start in 2 seconds... >> "%LOG_FILE%"
timeout /t 2 > nul

build\Release\crossmon.exe -i 1500 -o "%APP_OUTPUT%" notepad.exe >> "%LOG_FILE%" 2>&1

echo.
echo === Monitoring Results ===
echo === Monitoring Results === >> "%LOG_FILE%"
if exist "%APP_OUTPUT%" (
    type "%APP_OUTPUT%"
    type "%APP_OUTPUT%" >> "%LOG_FILE%"
) else (
    echo No results file found. Check for errors.
    echo No results file found. Check for errors. >> "%LOG_FILE%"
)

echo.
echo Test completed! Results saved to:
echo - Log file: %LOG_FILE%
echo - Monitoring output: %APP_OUTPUT%
pause
