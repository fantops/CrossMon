@echo off
REM CrossMon - NPU Detection Test for Windows

setlocal
set BUILD_DIR=build
set RESULT_FILE=results\npu_detection_win.txt

if not exist %BUILD_DIR% mkdir %BUILD_DIR%
if not exist results mkdir results
cd %BUILD_DIR%

REM Build the NPU test
cmake ..
if errorlevel 1 goto :fail
msbuild test_npu_monitor_win.vcxproj /p:Configuration=Release
if errorlevel 1 goto :fail

REM Run the test and save output
cd ..
%BUILD_DIR%\test_npu_monitor_win.exe > %RESULT_FILE% 2>&1

echo NPU detection test completed. Results saved to %RESULT_FILE%.
goto :eof

:fail
echo Build or test failed. See output above.
endlocal
exit /b 1 