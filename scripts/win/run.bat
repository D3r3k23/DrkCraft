@echo OFF

if "%1" == "--help" (
    echo Usage: scripts\build.bat [--help] build_config [--dev]
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

call scripts\win\get_build_config.bat %1 %2
%exe% %2
