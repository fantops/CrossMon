@echo off
REM Release build script for CrossMon

REM Set up MSVC environment
call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

REM Create build directory
if not exist "build_release" mkdir build_release
cd build_release

REM Configure with CMake for Release build
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release

REM Build the project in Release mode
cmake --build . --config Release

if %errorlevel% equ 0 (
    echo.
    echo --- Release Build successful ---
    echo To run: Release\crossmon.exe [options]
    echo.
) else (
    echo.
    echo --- Release Build failed ---
    echo.
)

cd ..
