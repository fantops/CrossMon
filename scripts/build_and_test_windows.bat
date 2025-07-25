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
    echo Error: CMake configuration failed! | tee -a ..\%LOG_FILE%
    pause
    exit /b 1
)

REM Build
cmake --build . --config %BUILD_TYPE% >> ..\%LOG_FILE% 2>&1
if errorlevel 1 (
    echo Error: Build failed! | tee -a ..\%LOG_FILE%
    pause
    exit /b 1
)
cd ..

echo Build completed successfully! | tee -a %LOG_FILE%

REM Step 2: Run unit tests
cd %BUILD_DIR%
ctest -C %BUILD_TYPE% --output-on-failure >> ..\%LOG_FILE% 2>&1
if errorlevel 1 (
    echo Warning: Some unit tests failed. Check log for details. | tee -a ..\%LOG_FILE%
) else (
    echo All unit tests passed! | tee -a ..\%LOG_FILE%
)
cd ..

echo.
echo Step 3: Quick system monitor test...
echo Step 3: Quick system monitor test... >> %LOG_FILE%
if exist %BUILD_DIR%\crossmon.exe (
    echo Running CrossMon for 15 seconds with 2s intervals... | tee -a %LOG_FILE%
    start /b /wait cmd /c "timeout /t 15 >nul & exit" | %BUILD_DIR%\crossmon.exe -i 2000 >> %LOG_FILE% 2>&1
    echo Quick test completed! | tee -a %LOG_FILE%
) else (
    echo Error: crossmon.exe not found in %BUILD_DIR%! | tee -a %LOG_FILE%
)

echo All tests completed! | tee -a %LOG_FILE%
echo Results saved to: %LOG_FILE%
pause
