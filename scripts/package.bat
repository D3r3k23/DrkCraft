@echo OFF

call scripts\get_build_config.bat --release

set /p version=<VERSION.txt

set "package_name=DrkCraft-%version%-win"
set "package_dir=packages\%package_name%"

echo Building package: %package_name%

call scripts\clean.bat
call scripts\build.bat --release
if NOT %errorlevel% == 0 (
    echo CMake build failed
    exit /b 1
)

if exist %package_dir% rmdir /S /Q %package_dir%
mkdir %package_dir%

echo Copying executable
xcopy /E %exe% %package_dir%

echo Copying data files
xcopy /E data %package_dir%\data\

echo Copying config files
xcopy /E config %package_dir%\config\
