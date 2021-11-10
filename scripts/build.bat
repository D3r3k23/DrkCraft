@echo OFF

call scripts\get_build_config.bat %1

echo Configuring CMake
cmake -S . -B %build_dir% -D CMAKE_BUILD_TYPE=%build_config%
if NOT %errorlevel% == 0 exit /b 1

echo Building DrkCraft
cmake --build %build_dir% --config %build_config%
if NOT %errorlevel% == 0 exit /b 1
