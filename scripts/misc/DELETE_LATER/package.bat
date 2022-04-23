@echo OFF

if "%1" == "--help" (
    echo Usage: scripts\win\package.bat [--help] build_config [--profile] [dev_mode] [trace_log]
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
    echo Default: --Dist
    exit /b 0
)

if "%1" == "" (
    call scripts\win\get_build_config.bat --dist
) else (
    call scripts\win\get_build_config.bat %1 %2 %3 %4
)

set "package_dir=packages\DrkCraft"
if exist %package_dir% rmdir /S /Q %package_dir%
mkdir %package_dir%

set /p version=<VERSION.txt

if "%dist%" == "ON" (
    set "package_name=DrkCraft-v%version%-Win-Dist"
) else (
    set "package_name=DrkCraft-v%version%-Win-%build_config%"
)
set "package_zip=packages\%package_name%.zip"

echo Building package: %package_name%

call scripts\win\clean.bat --all
call scripts\win\build.bat %1 %2 %3 %4
if NOT %errorlevel% == 0 (
    exit /b 1
)

echo Copying executable
xcopy %exe% %package_dir%

echo Copying assets
xcopy /e assets %package_dir%\assets\
if "%dist%" == "ON" (
    rmdir /S /Q %package_dir%\assets\images\textures\blocks
)

echo Creating config directory
xcopy /e config\default %package_dir%\config\

echo Creating data directory
mkdir %package_dir%\data
mkdir %package_dir%\data\logs
mkdir %package_dir%\data\profile

if NOT "%dist%" == "ON" (
    echo Creating tools directory
    mkdir %package_dir%\tools
    xcopy scripts\gen_block_texture_atlas.py %package_dir%\tools\
    xcopy scripts\clean_logs.py            %package_dir%\tools\
    xcopy scripts\profile.py             %package_dir%\tools\
    xcopy scripts\requirements.txt     %package_dir%\tools\

    xcopy data\blocks.yaml %package_dir%\data\
)

echo Creating about directory
mkdir %package_dir%\about

echo Writing README.txt
python scripts\packaging\write_package_readme.py %package_dir%\about\README.txt ^
    %version% %exe%

echo Writing build.txt
python scripts\packaging\write_build_file.py %package_dir%\about\build.txt ^
    %version% Windows %build_config% %en_profile% %en_dev_mode% %en_trace_log% %dist%

echo Writing LICENSE.txt
python scripts\packaging\write_license_file.py %package_dir%\about\LICENSE.txt LICENSE.txt lib\LICENSE.txt

echo Creating archive %package_zip%
if exist %package_zip% del /Q %package_zip%
tar -a -c -f %package_zip% -C packages DrkCraft

echo Package %package_name% built
