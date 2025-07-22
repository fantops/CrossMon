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
echo 1. Build and Test Everything
echo 2. Quick System Monitor Test
echo 3. GPU Detection Test
echo 4. Application Monitoring Test (Notepad)
echo 5. Show Help
echo 6. View Results Folder
echo 7. Exit
echo.
set /p choice="Enter your choice (1-7): "

if "%choice%"=="1" goto BUILD_TEST
if "%choice%"=="2" goto QUICK_TEST
if "%choice%"=="3" goto GPU_TEST
if "%choice%"=="4" goto APP_TEST
if "%choice%"=="5" goto HELP_TEST
if "%choice%"=="6" goto VIEW_RESULTS
if "%choice%"=="7" goto EXIT
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
