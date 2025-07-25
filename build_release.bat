@echo off
REM Release build script for CrossMon

echo Building CrossMon Release...

REM Create build directory
if not exist "build_release" mkdir build_release
cd build_release

echo Configuring with CMake...
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release

if %errorlevel% neq 0 (
    echo CMake configuration failed!
    cd ..
    exit /b 1
)

echo Building project...
cmake --build . --config Release

if %errorlevel% equ 0 (
    echo.
    echo --- Release Build successful ---
    echo To run: Release\crossmon.exe [options]
    echo.
) else (
    echo.
    echo --- Release Build failed ---
    echo Check the output above for errors.
    echo.
)

cd ..
