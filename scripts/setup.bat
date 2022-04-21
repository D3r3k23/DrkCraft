@echo OFF

if "%1" == "--clean" (
    set "clean=true"
) else (
    set "clean=false"
)

set "venv=.venv"

if exist %venv% (
    if "%clean%" == "true" (
        rmdir /s /q %venv%
        set "make_venv=true"
    ) else (
        set "make_venv=false"
    )
) else (
    set "make_venv=true"
)

if "%make_venv%" == "true" (
    python -m venv %venv%
)

call %venv%\Scripts\activate

mkdir scripts\drkcraft-py\build
pip install -e scripts\drkcraft-py

pip install --upgrade -r scripts\requirements.txt
pip install --upgrade -r launcher\requirements.txt
