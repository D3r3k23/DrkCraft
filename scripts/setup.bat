@echo OFF

set "clean=false"
if "%1" == "--clean"   set "clean=true"
if "%2" == "--clean"   set "clean=true"

set "no_venv=false"
if "%1" == "--no-venv" set "no_venv=true"
if "%2" == "--no-venv" set "no_venv=true"

if "%no_venv%" == "false" (
    set "venv=.venv"

    if exist %venv% (
        if "%clean%" == "true" (
            echo Cleaning virtualenv
            rmdir /s /q %venv%
            set "make_venv=true"
        ) else (
            set "make_venv=false"
        )
    ) else (
        set "make_venv=true"
    )

    if "%make_venv%" == "true" (
        echo Creating virtualenv
        python -m venv %venv%
    )

    echo Activating virtualenv
    call %venv%\Scripts\activate
)

echo Building drkcraft-py
if not exist scripts\drkcraft-py\build mkdir scripts\drkcraft-py\build
pip install -q -e scripts\drkcraft-py

echo Installing dependencies
pip install --upgrade -r scripts\requirements.txt
pip install --upgrade -r launcher\requirements.txt
