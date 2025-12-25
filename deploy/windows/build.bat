@echo off
REM SmithTool Windows Build and Package Script
REM Run this script in Developer Command Prompt or with Qt environment set

setlocal enabledelayedexpansion

echo ===================================
echo SmithTool Windows Build Script
echo ===================================
echo.

REM Check if Qt path is set
if "%CMAKE_PREFIX_PATH%"=="" (
    echo ERROR: CMAKE_PREFIX_PATH not set!
    echo Please set it to your Qt installation, e.g.:
    echo   set CMAKE_PREFIX_PATH=C:\Qt\6.5.0\msvc2019_64
    exit /b 1
)

REM Get script directory
set SCRIPT_DIR=%~dp0
set PROJECT_DIR=%SCRIPT_DIR%..\..
set BUILD_DIR=%PROJECT_DIR%\build-release
set DEPLOY_DIR=%PROJECT_DIR%\deploy\release

echo Project directory: %PROJECT_DIR%
echo Build directory: %BUILD_DIR%
echo Deploy directory: %DEPLOY_DIR%
echo.

REM Create build directory
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd /d "%BUILD_DIR%"

echo [1/4] Generating build files...
cmake "%PROJECT_DIR%" -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 (
    echo ERROR: CMake configuration failed!
    exit /b 1
)

echo.
echo [2/4] Building...
nmake
if errorlevel 1 (
    echo ERROR: Build failed!
    exit /b 1
)

echo.
echo [3/4] Deploying Qt dependencies...
if not exist "%DEPLOY_DIR%" mkdir "%DEPLOY_DIR%"
copy /Y SmithTool.exe "%DEPLOY_DIR%\"
cd /d "%DEPLOY_DIR%"
windeployqt --release --no-translations SmithTool.exe
if errorlevel 1 (
    echo WARNING: windeployqt failed, trying alternative...
)

echo.
echo [4/4] Build complete!
echo.
echo Output files are in: %DEPLOY_DIR%
echo.

REM Show file list
echo Files created:
dir /b "%DEPLOY_DIR%\*.exe"
dir /b "%DEPLOY_DIR%\*.dll" 2>nul

echo.
echo To create installer, run Inno Setup with:
echo   iscc "%SCRIPT_DIR%\smithtool.iss"
echo.

endlocal
pause
