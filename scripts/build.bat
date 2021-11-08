@echo OFF

if "%1" == ""          set "build_config=Debug"
if "%1" == "--debug"   set "build_config=Debug"
if "%1" == "-D"        set "build_config=Debug"
if "%1" == "--release" set "build_config=Release"
if "%1" == "-R"        set "build_config=Release"

set "build_dir=build"

echo Configuring CMake
cmake -S . -B %build_dir% -D CMAKE_BUILD_TYPE=%build_config%
if NOT %errorlevel% == 0 exit /b 1

echo Building DrkCraft
cmake --build %build_dir% --config %build_config%
if NOT %errorlevel% == 0 exit /b 1
