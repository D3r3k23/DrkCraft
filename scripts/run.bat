@echo OFF

set "print_help=OFF"
if "%1" == "-h"     set "print_help=ON"
if "%1" == "--help" set "print_help=ON"
if "%print_help%" == "ON" (
    echo Usage: scripts\build.bat [-h] build_config [dev_mode]
    echo.
    echo Positional arguments:
    echo   build_config   --Debug, --Release, --RelWithDebInfo, --Dist
    echo                  Dist mode overrides other settings
    echo.
    echo Optional arguments:
    echo   --help         Show this help message and exit
    echo   --dev          Enable dev mode
    echo.
    echo Default: --Debug --dev
    exit /b 0
)

call scripts\get_build_config.bat %1 %2
%exe% %2
