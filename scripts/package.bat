@echo OFF

set "package_dir=packages\DrkCraft"

if exist %package_dir% rmdir /S /Q %package_dir%
mkdir %package_dir%

call scripts\get_build_config.bat %1 %2 %3

set /p version=<VERSION.txt

set "package_name=DrkCraft-v%version%-%build_config%-win"
set "package_zip=packages\%package_name%.zip"

echo Building package: %package_name%

call scripts\clean.bat
call scripts\build.bat %1 %2 %3
if NOT %errorlevel% == 0 (
    echo CMake build failed
    exit /b 1
)

echo Copying executable
xcopy /e %exe% %package_dir%

echo Copying assets
xcopy /e assets %package_dir%\assets\

echo Creating config directory
xcopy /e config %package_dir\config\default\

echo Creating data directory
xcopy /e /t data %package_dir%\data\

echo Creating tools directory
mkdir %package_dir%\tools
copy scripts\clean_logs.py        %package_dir%\tools\ >NUL
copy scripts\gen_texture_atlas.py %package_dir%\tools\ >NUL

echo Creating about directory
mkdir %package_dir%\about
copy LICENSE.txt     %package_dir%\about\LICENSE.txt     >NUL
copy lib\LICENSE.txt %package_dir%\about\LICENSE_LIB.txt >NUL

echo Writing build.txt
python scripts\write_build_file.py %package_dir%\about\build.txt ^
    %version% Windows %build_config% %en_profile% %en_dev_mode% %dist%

echo Creating archive %package_zip%
if exist %package_zip% del /Q %package_zip%
tar -a -c -f %package_zip% -C "packages" "DrkCraft"

echo Package %package_name% built
