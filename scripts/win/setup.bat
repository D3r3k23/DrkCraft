@echo OFF

if NOT exist venv (
    python -m venv venv
)
call venv\Scripts\activate
pip install --upgrade -q -r scripts\requirements.txt --disable-pip-version-check
deactivate
