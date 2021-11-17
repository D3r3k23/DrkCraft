@echo OFF

@REM  Defaults

if "%1" == "" (
    set "build_config=Debug"
    set "en_profile=ON"
    set "en_dev_mode=ON"
    set "dist=OFF"

) else (

    @REM Dist mode

    if "%1" == "--dist" set "dist=ON"
    if "%1" == "--Dist" set "dist=ON"

    if "%dist%" == "ON" (
        set "build_config=Release"
        set "en_profile=OFF"
        set "en_dev_mode=OFF"

    ) else (

        @REM Set build_config

        if "%1" == "--debug" set "build_config=Debug"
        if "%1" == "--Debug" set "build_config=Debug"
        if "%1" == "-D"      set "build_config=Debug"

        if "%1" == "--release" set "build_config=Release"
        if "%1" == "--Release" set "build_config=Release"
        if "%1" == "-R"        set "build_config=Release"

        if "%1" == "--RelWithDebInfo" set "build_config=RelWithDebInfo"

        @REM Set en_profile

        set "en_profile=OFF"
        if "%2" == "--profile" set "en_profile=ON"
        if "%3" == "--profile" set "en_profile=ON"

        @REM Set en_dev_mode

        set "en_dev_mode=OFF"
        if "%2" == "--dev" set "en_dev_mode=ON"
        if "%3" == "--dev" set "en_dev_mode=ON"
    )
)

set "build_dir=build"
set "exe=%build_dir%\%build_config%\DrkCraft.exe"
