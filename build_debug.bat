@echo off
REM Debug build script for CrossMon

REM Set up MSVC environment
call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

REM Create build directory
if not exist "build_debug" mkdir build_debug
cd build_debug

REM Configure with CMake for Debug build
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Debug

REM Build the project in Debug mode
cmake --build . --config Debug

if %errorlevel% equ 0 (
    echo.
    echo --- Debug Build successful ---
    echo To run with debug output: Debug\crossmon.exe [options]
    echo.
) else (
    echo.
    echo --- Debug Build failed ---
    echo.
)

cd ..
