@echo OFF

call scripts\get_build_config.bat %1 %2 %3

echo ====== Configuring CMake ======
cmake -S . -B %build_dir%              ^
    -D CMAKE_BUILD_TYPE=%build_config% ^
    -D DRK_EN_PROFILE=%en_profile%     ^
    -D DRK_EN_DEV_MODE=%en_dev_mode%

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
