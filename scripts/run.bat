@echo OFF

if "%1" == ""          set "build_type=Debug"
if "%1" == "--debug"   set "build_type=Debug"
if "%1" == "-D"        set "build_type=Debug"
if "%1" == "--release" set "build_type=Release"
if "%1" == "-R"        set "build_type=Release"

set "build_dir=build"

%build_dir%\%build_type%\DrkCraft.exe
