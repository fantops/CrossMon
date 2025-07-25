@echo off
REM Debug build script for CrossMon

echo Building CrossMon Debug...

REM Create build directory
if not exist "build_debug" mkdir build_debug
cd build_debug

echo Configuring with CMake...
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Debug

if %errorlevel% neq 0 (
    echo CMake configuration failed!
    cd ..
    exit /b 1
)

echo Building project...
cmake --build . --config Debug

if %errorlevel% equ 0 (
    echo.
    echo --- Debug Build successful ---
    echo To run with debug output: Debug\crossmon.exe [options]
    echo.
) else (
    echo.
    echo --- Debug Build failed ---
    echo Check the output above for errors.
    echo.
)

cd ..
