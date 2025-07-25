@echo off
REM Release build script for CrossMon

echo Setting up Visual Studio environment...

REM Try to find Visual Studio installation using vswhere.exe
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if exist "%VSWHERE%" (
    for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
        set "VSINSTALLDIR=%%i"
    )
)

REM Set up MSVC environment - try multiple paths
if defined VSINSTALLDIR (
    echo Found Visual Studio at: %VSINSTALLDIR%
    call "%VSINSTALLDIR%\VC\Auxiliary\Build\vcvars64.bat"
) else (
    echo Trying common Visual Studio paths...
    if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat" (
        echo Found VS 2022 Enterprise
        call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
    ) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" (
        echo Found VS 2022 Professional
        call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
    ) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
        echo Found VS 2022 Community
        call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    ) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat" (
        echo Found VS 2019 Enterprise
        call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
    ) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars64.bat" (
        echo Found VS 2019 Professional
        call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars64.bat"
    ) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" (
        echo Found VS 2019 Community
        call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
    ) else (
        echo Warning: Could not find Visual Studio installation.
        echo Attempting to build without setting up MSVC environment...
        echo CMake should still work if VS Build Tools are available.
    )
)

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
