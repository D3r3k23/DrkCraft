@echo OFF

set "run_src=false"
set "run_bin=false"
if "%2" == "--src" (
    set "run_src=true"
) else (
    set "run_bin=true"
)

if "%1" == "installer" (
    if "%run_src%" == "true" (
        python -m src\main.py --debug --install=Installations
    ) else (
        build\bin\drkcraft-launcher.exe --debug --install=Installations
    )
) else (
    if "%1" == "launcher" (
        if "%run_src%" == "true" (
            python -m src\main.py --debug --install=Installations --launch
        ) else (
            build\bin\drkcraft-launcher.exe --debug --install=Installations --launch
        )
    ) else (
        echo usage: run.bat <installer | launcher>
    )
)
