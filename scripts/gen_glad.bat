@echo OFF

python -m venv venv
call venv\Scripts\activate
pip install --upgrade -q --disable-pip-version-check glad

python -m glad --generator c --no-loader --out-path lib\glad

deactivate
