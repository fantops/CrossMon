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

REM Create results directory if it doesn't exist
if not exist results mkdir results

REM Set output file in results directory with timestamp
set "CONSOLE_LOG=results\crossmon_console_quick_%date:~-4,4%%date:~-10,2%%date:~-7,2%_%time:~0,2%%time:~3,2%%time:~6,2%.log"
set "CONSOLE_LOG=%CONSOLE_LOG: =0%"

REM Initialize console log
echo. > "%CONSOLE_LOG%"
echo CrossMon Quick Test Console Output >> "%CONSOLE_LOG%"
echo Generated: %date% %time% >> "%CONSOLE_LOG%"
echo ======================================== >> "%CONSOLE_LOG%"
echo. >> "%CONSOLE_LOG%"

REM Get the full path to the executable
set "CROSSMON_EXE=%CD%\build\Release\crossmon.exe"
echo Using executable: %CROSSMON_EXE%
echo Using executable: %CROSSMON_EXE% >> "%LOG_FILE%"
echo Console output will be saved to: %CONSOLE_LOG%
echo Console output will be saved to: %CONSOLE_LOG% >> "%LOG_FILE%"

REM Start CrossMon with console output capture
echo Starting CrossMon with console output capture...
echo Starting CrossMon with console output capture... >> "%LOG_FILE%"
start /min "" cmd /c ""%CROSSMON_EXE%" -i 2000 >> "%CONSOLE_LOG%" 2>&1"

REM Wait a moment for process to start
timeout /t 2 /nobreak > nul

REM Check if process started successfully
tasklist | findstr /i crossmon > nul
if errorlevel 1 (
    echo Error: CrossMon failed to start!
    echo Error: CrossMon failed to start! >> "%LOG_FILE%"
    exit /b 1
)

echo CrossMon is running... monitoring for 15 seconds
echo CrossMon is running... monitoring for 15 seconds >> "%LOG_FILE%"

REM Wait for monitoring (13 more seconds since we already waited 2)
timeout /t 13 /nobreak

REM Stop the process - try graceful first, then force
echo Stopping CrossMon...
echo Stopping CrossMon... >> "%LOG_FILE%"
taskkill /im crossmon.exe /t
if errorlevel 1 (
    echo Graceful shutdown failed, forcing termination...
    taskkill /f /im crossmon.exe /t
)

REM Wait a moment for process to fully terminate
timeout /t 1 > nul

echo.
echo Test completed!
echo Test completed! >> "%LOG_FILE%"
echo Console output saved to: %CONSOLE_LOG%
echo Console output saved to: %CONSOLE_LOG% >> "%LOG_FILE%"
if "%PARENT_LOG%"=="" (
    echo Results saved to: %LOG_FILE%
    pause
)
