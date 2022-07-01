@echo OFF

set "clean=false"
if "%1" == "--clean" set "clean=true"
if "%2" == "--clean" set "clean=true"
if "%3" == "--clean" set "clean=true"

set "no_venv=false"
if "%1" == "--no-venv" set "no_venv=true"
if "%2" == "--no-venv" set "no_venv=true"
if "%3" == "--no-venv" set "no_venv=true"

set "update=false"
if "%1" == "--update" set "update=true"
if "%2" == "--update" set "update=true"
if "%3" == "--update" set "update=true"

set "venv=.venv"

if "%no_venv%" == "false" (
    if exist "%venv%" (
        if "%clean%" == "true" (
            if defined VIRTUAL_ENV (
                echo Deactivating %venv%
                call deactivate
            )
            echo Deleting %venv%
            rmdir /s /q %venv%
        )
    )
    if NOT exist "%venv%" (
        echo Creating %venv%
        python -m venv %venv%
    )
    if NOT defined VIRTUAL_ENV (
        echo Activating %venv%
        call %venv%\Scripts\activate
    )
)

set "drkcraft_egg_link=%venv%\Lib\site-packages\drkcraft-py.egg-link"

if NOT exist "%drkcraft_egg_link%" (
    set "build_drkcraft=true"
) else (
    set "build_drkcraft=false"
)

if "%update%" == "true" (
    set "install_packages=true"
) else (
    if "%no_venv%" == "true" (
        set "install_packages=true"
    ) else (
        if "%build_drkcraft%" == "true" (
            set "install_packages=true"
        ) else (
            set "install_packages=false"
        )
    )
)

if "%build_drkcraft%" == "true" (
    echo Building drkcraft-py package
    pip install -e python
)
if "%install_packages%" == "true" (
    echo Installing drkcraft-py packages
    pip install --upgrade -r python\requirements.txt

    echo Installing launcher packages
    pip install --upgrade -r launcher\requirements.txt
    pip install --upgrade -r launcher\require-types.txt

    echo Installing std_usage packages
    pip install --upgrade -r tools\std_usage\requirements.txt
)
