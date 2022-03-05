@echo OFF

set "package_dir=packages\DrkCraft"

if exist %package_dir% rmdir /S /Q %package_dir%
mkdir %package_dir%

call scripts\win\get_build_config.bat %1 %2 %3 %4

set /p version=<VERSION.txt

set "package_name=DrkCraft-v%version%-%build_config%-Win"
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
mkdir %package_dir%\data\saves
xcopy data\blocks.yaml %package_dir%\data\

if "%dist%" != "ON" (
    echo Creating tools directory
    mkdir %package_dir%\tools
    xcopy scripts\gen_texture_atlas.py %package_dir%\tools\
    xcopy scripts\clean_logs.py        %package_dir%\tools\
    xcopy scripts\profile.py           %package_dir%\tools\
)

echo Creating about directory
mkdir %package_dir%\about

echo Write README.txt
python scripts\packaging\write_package_readme.py %package_dir%\about\README.txt ^
    %version%

echo Writing build.txt
python scripts\packaging\write_build_file.py %package_dir%\about\build.txt ^
    %version% Windows %build_config% %en_profile% %en_dev_mode% %en_trace_log% %dist%

echo Writing LICENSE.txt
python scripts\packaging\write_license_file.py %package_dir%\about\LICENSE.txt LICENSE.txt lib\LICENSE.txt

echo Creating doc directory
xcopy /e doc %package_dir%\about\doc\

echo Creating package archive %package_zip%
if exist %package_zip% del /Q %package_zip%
tar -a -c -f %package_zip% -C packages DrkCraft

echo Package %package_name% built

FOR %%A IN (%package_zip%) DO set size=%%~zA
echo Package archive size: %size% bytes
