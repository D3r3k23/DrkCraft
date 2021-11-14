@echo OFF

call scripts\get_build_config.bat --release

set /p version=<VERSION.txt

set "package_name=DrkCraft-v%version%-win"
set "package_dir=packages\%package_name%"
set "package_zip=packages\%package_name%.zip"

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
xcopy /e %exe% %package_dir%

echo Copying assets
xcopy /e assets %package_dir%\assets\

echo Copying config files
xcopy /e config %package_dir%\config\

echo Creating data directory
mkdir %package_dir%\data\logs
mkdir %package_dir%\data\saves

echo Copying licenses
copy LICENSE.txt %package_dir%\data\ >NUL
copy lib\LICENSE.txt %package_dir%\data\LICENSE_LIB.txt >NUL

echo Creating archive %package_zip%
if exist %package_zip% del /Q %package_zip%
tar -a -c -f %package_zip% -C "packages" %package_name%

echo Package %package_name% built
