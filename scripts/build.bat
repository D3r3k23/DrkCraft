@echo OFF

call scripts\get_build_config.bat %1

if "%2" == "--profile" (
    set profile=ON
) else (
    set profile=OFF
)

echo ====== Configuring CMake ======
cmake -S . -B %build_dir% -D CMAKE_BUILD_TYPE=%build_config% -D DRK_PROFILE=%profile%
if NOT %errorlevel% == 0 exit /b 1

echo ====== Building DrkCraft ======
cmake --build %build_dir% --config %build_config%
if NOT %errorlevel% == 0 exit /b 1
