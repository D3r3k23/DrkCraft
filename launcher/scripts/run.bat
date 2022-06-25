@echo OFF

set "build_dir=build"
set "bin_dir=%build_dir%\pyinstaller\bin"

set "exe_name=drkcraft-launcher.exe"
set "exe=%bin_dir%\%exe_name%"
set "main=src\main.py"

set "run_src=false"
set "run_bin=false"
if "%2" == "--src" (
    set "run_src=true"
) else (
    if NOT EXIST "%exe%" (
        echo Error: executable not found: running from source
        set "run_src=true"
    )
    set "run_bin=true"
)

set "install_dir=test_install_dir"

if "%1" == "installer" (
    if "%run_src%" == "true" (
        python %main% --debug --install=%install_dir%
    ) else (
        %exe% --debug --install=%install_dir%
    )
) else (
    if "%1" == "launcher" (
        if "%run_src%" == "true" (
            python %main% --debug --install=%install_dir% --launch
        ) else (
            %exe% --debug --install=%install_dir% --launch
        )
    ) else (
        echo usage: run.bat ^<installer ^| launcher^> [--src]
    )
)
