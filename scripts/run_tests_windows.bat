@echo off
REM CrossMon - Test Launcher for Windows
REM Simple menu to run different tests

REM Create results directory if it doesn't exist
if not exist results mkdir results

:MENU
cls
echo ========================================
echo CrossMon - Test Launcher
echo ========================================
echo.
echo Choose a test to run:
echo.
echo 1. Build and Test Everything (includes unit tests)
echo 2. Quick System Monitor Test
echo 3. GPU Detection Test
echo 4. Application Monitoring Test (Notepad)
echo 5. Unit Tests Only
echo 6. Show Help
echo 7. View Results Folder
echo 8. Exit
echo.
set /p choice="Enter your choice (1-8): "

if "%choice%"=="1" goto BUILD_TEST
if "%choice%"=="2" goto QUICK_TEST
if "%choice%"=="3" goto GPU_TEST
if "%choice%"=="4" goto APP_TEST
if "%choice%"=="5" goto UNIT_TEST
if "%choice%"=="6" goto HELP_TEST
if "%choice%"=="7" goto VIEW_RESULTS
if "%choice%"=="8" goto EXIT
echo Invalid choice. Please try again.
pause
goto MENU

:BUILD_TEST
echo Running complete build and test suite...
echo All output will be logged to results folder.
echo.
call build_and_test_windows.bat
pause
goto MENU

:QUICK_TEST
echo Running quick system monitor test...
echo Output will be logged to results folder.
echo.
call test_quick_windows.bat
pause
goto MENU

:GPU_TEST
echo Running GPU detection test...
echo Output will be logged to results folder.
echo.
call test_gpu_detection_windows.bat
pause
goto MENU

:APP_TEST
echo Running application monitoring test...
echo Output will be logged to results folder.
echo.
call test_app_monitoring_windows.bat
pause
goto MENU

:UNIT_TEST
echo Running unit tests only...
echo.
cd /d "%~dp0\.."
if not exist build (
    echo Build directory not found. Building project first...
    echo.
    mkdir build
    cmake -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build --config Release
)
echo.
echo Running all unit tests (including new GPU tests)...
cd build
ctest -C Release --output-on-failure
cd ..
echo.
pause
goto MENU

:VIEW_RESULTS
echo Opening results folder...
cd /d "%~dp0\.."
if exist "results" (
    explorer results
    echo Results folder opened in Windows Explorer.
) else (
    echo No results folder found. Run some tests first.
)
pause
goto MENU

:HELP_TEST
cd /d "%~dp0\.."
if exist "build\Release\crossmon.exe" (
    build\Release\crossmon.exe --help
) else (
    echo CrossMon not built yet. Please run option 1 first.
)
echo.
pause
goto MENU

:EXIT
echo Goodbye!
exit /b 0
