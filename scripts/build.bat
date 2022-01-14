@echo OFF

set "print_help=OFF"
if "%1" == "-h"     set "print_help=ON"
if "%1" == "--help" set "print_help=ON"
if "%print_help%" == "ON" (
    echo Usage: scripts\build.bat [-h] build_config [profile] [dev_mode] [trace_log]
    echo.
    echo Positional arguments:
    echo   build_config   --Debug, --Release, --RelWithDebInfo, --Dist
    echo                  Dist mode overrides other settings
    echo.
    echo Optional arguments:
    echo   --help         Show this help message and exit
    echo   --profile      Enable profiling
    echo   --dev          Enable dev mode
    echo   --trace        Enable trace log console output
    echo                  Only available in Debug builds
    echo.
    echo Default: --Debug --profile --dev
    exit /b 0
)

call scripts\get_build_config.bat %1 %2 %3 %4

echo ====== Configuring CMake ======
cmake -S . -B %build_dir%                  ^
    -D CMAKE_BUILD_TYPE=%build_config%     ^
    -D DRK_EN_PROFILE=%en_profile%         ^
    -D DRK_EN_DEV_MODE=%en_dev_mode%       ^
    -D DRK_EN_TRACE_LOGGING=%en_trace_log% ^
    -D DRK_DIST=%dist%

if NOT %errorlevel% == 0 (
    echo CMake config failed
    exit /b 1
)

echo ====== Building DrkCraft ======
cmake --build %build_dir% --config %build_config%

if NOT %errorlevel% == 0 (
    echo CMake build failed
    exit /b 1
)

echo ====== DrkCraft build completed ======
