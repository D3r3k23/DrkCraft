@echo OFF

@REM Defaults
if "%1" == "" (
    set "build_config=Debug"
    set "en_profile=ON"
    set "en_dev_mode=ON"
    set "en_trace_log=OFF"
    set "dist=OFF"

) else (

    @REM Dist mode

    if "%1" == "--dist" set "dist=ON"
    if "%1" == "--Dist" set "dist=ON"

    if "%dist%" == "ON" (
        set "build_config=Release"
        set "en_profile=OFF"
        set "en_dev_mode=OFF"
        set "en_trace_log=OFF"

    ) else (

        @REM Set build_config
        set "build_config=None"

        if "%1" == "--debug" set "build_config=Debug"
        if "%1" == "--Debug" set "build_config=Debug"
        if "%1" == "-D"      set "build_config=Debug"

        if "%1" == "--release" set "build_config=Release"
        if "%1" == "--Release" set "build_config=Release"
        if "%1" == "-R"        set "build_config=Release"

        if "%1" == "--RelWithDebInfo" set "build_config=RelWithDebInfo"

        if "%build_config%" == "None" set "build_config=Debug"

        @REM Set en_profile
        set "en_profile=OFF"
        if "%2" == "--profile" set "en_profile=ON"
        if "%3" == "--profile" set "en_profile=ON"
        if "%4" == "--profile" set "en_profile=ON"

        @REM Set en_dev_mode
        set "en_dev_mode=OFF"
        if "%2" == "--dev" set "en_dev_mode=ON"
        if "%3" == "--dev" set "en_dev_mode=ON"
        if "%4" == "--dev" set "en_dev_mode=ON"

        @REM Set en_trace_log
        set "en_trace_log=OFF"
        if "%build_config%" == "Debug" (
            if "%2" == "--trace" set "en_trace_log=ON"
            if "%3" == "--trace" set "en_trace_log=ON"
            if "%4" == "--trace" set "en_trace_log=ON"
        )
    )
)

set "build_dir=build"
set "exe=%build_dir%\%build_config%\DrkCraft.exe"
