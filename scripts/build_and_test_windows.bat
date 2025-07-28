@echo off
REM CrossMon - Unified Build, Test, and Quick Monitor Script for Windows
REM Usage: build_and_test_windows.bat [debug|release]

setlocal
set BUILD_TYPE=Release
if not "%1"=="" (
    if /I "%1"=="debug" set BUILD_TYPE=Debug
)

set LOG_FILE=results\build_and_test_%BUILD_TYPE%.log

REM Step 1: Build
if not exist build_release mkdir build_release
if not exist build_debug mkdir build_debug
if /I "%BUILD_TYPE%"=="Debug" (
    set BUILD_DIR=build_debug
) else (
    set BUILD_DIR=build_release
)
cd %BUILD_DIR%

REM Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=%BUILD_TYPE% > ..\%LOG_FILE% 2>&1
if errorlevel 1 (
    echo Error: CMake configuration failed!
    echo Error: CMake configuration failed! >> ..\%LOG_FILE%
    pause
    exit /b 1
)

REM Build
cmake --build . --config %BUILD_TYPE% >> ..\%LOG_FILE% 2>&1
if errorlevel 1 (
    echo Error: Build failed!
    echo Error: Build failed! >> ..\%LOG_FILE%
    pause
    exit /b 1
)
cd ..

echo Build completed successfully!
echo Build completed successfully! >> %LOG_FILE%

REM Step 2: Run unit tests
cd %BUILD_DIR%
ctest -C %BUILD_TYPE% --output-on-failure >> ..\%LOG_FILE% 2>&1
if errorlevel 1 (
    echo Warning: Some unit tests failed. Check log for details.
    echo Warning: Some unit tests failed. Check log for details. >> ..\%LOG_FILE%
) else (
    echo All unit tests passed!
    echo All unit tests passed! >> ..\%LOG_FILE%
)
cd ..

echo.
echo Step 3: Quick system monitor test...
echo Step 3: Quick system monitor test... >> %LOG_FILE%
if exist %BUILD_DIR%\%BUILD_TYPE%\crossmon.exe (
    echo Running CrossMon for 15 seconds with 2s intervals...
    echo Running CrossMon for 15 seconds with 2s intervals... >> %LOG_FILE%
    
    REM Create console output log file
    set CONSOLE_LOG=results\crossmon_console_%BUILD_TYPE%.log
    if not exist results mkdir results
    echo. > %CONSOLE_LOG%
    echo CrossMon Console Output Log - %BUILD_TYPE% Build >> %CONSOLE_LOG%
    echo Generated: %date% %time% >> %CONSOLE_LOG%
    echo ======================================== >> %CONSOLE_LOG%
    echo. >> %CONSOLE_LOG%
    
    REM Start CrossMon and capture console output
    echo Starting CrossMon with console output capture...
    echo Starting CrossMon with console output capture... >> %LOG_FILE%
    start /min "" cmd /c ""%BUILD_DIR%\%BUILD_TYPE%\crossmon.exe" -i 2000 >> "%CONSOLE_LOG%" 2>&1"
    
    REM Wait a moment for process to start
    timeout /t 2 >nul
    
    REM Check if process started successfully
    tasklist | findstr /i crossmon > nul
    if errorlevel 1 (
        echo Warning: CrossMon may not have started properly
        echo Warning: CrossMon may not have started properly >> %LOG_FILE%
    ) else (
        echo CrossMon is running, monitoring for 13 more seconds...
        echo CrossMon is running, monitoring for 13 more seconds... >> %LOG_FILE%
    )
    
    REM Wait for monitoring (13 more seconds since we already waited 2)
    timeout /t 13 >nul
    
    REM Stop the process - try graceful first, then force
    echo Stopping CrossMon...
    echo Stopping CrossMon... >> %LOG_FILE%
    taskkill /im crossmon.exe /t >nul 2>&1
    if errorlevel 1 (
        echo Forcing termination...
        echo Forcing termination... >> %LOG_FILE%
        taskkill /f /im crossmon.exe /t >nul 2>&1
    )
    
    REM Wait for full termination
    timeout /t 1 >nul
    
    echo Quick test completed!
    echo Quick test completed! >> %LOG_FILE%
    echo Console output saved to: %CONSOLE_LOG%
    echo Console output saved to: %CONSOLE_LOG% >> %LOG_FILE%
) else (
    echo Error: crossmon.exe not found in %BUILD_DIR%\%BUILD_TYPE%\!
    echo Error: crossmon.exe not found in %BUILD_DIR%\%BUILD_TYPE%\! >> %LOG_FILE%
)

echo All tests completed!
echo All tests completed! >> %LOG_FILE%
echo Results saved to: %LOG_FILE%
pause
