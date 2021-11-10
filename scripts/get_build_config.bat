@echo OFF

if "%1" == ""          set "build_config=Debug"
if "%1" == "--debug"   set "build_config=Debug"
if "%1" == "-D"        set "build_config=Debug"
if "%1" == "--release" set "build_config=Release"
if "%1" == "-R"        set "build_config=Release"

set "build_dir=build"

set "exe=%build_dir%\%build_config%\DrkCraft.exe
