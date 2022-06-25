# X11 development package is required to compile GLFW
# - on Debian (Ubuntu & Linux Mint): sudo apt install xorg-dev

if [ "$1" == "--clean" ] || [ "$2" == "--clean" ] || [ "$3" == "--clean" ]; then
    clean="true"
else
    clean="false"
fi
if [ "$1" == "--no-venv" ] || [ "$2" == "--no-venv" ] || [ "$3" == "--no-venv" ]; then
    no_venv="true"
else
    no_venv="false"
fi
if [ "$1" == "--update" ] || [ "$2" == "--update" ] || [ "$3" == "--update" ]; then
    update="true"
else
    update="false"
fi

venv=".venv"

if [ "$no_venv" == "false" ]; then
    if [ -d $venv ]; then
        if [ "$clean" == "true" ]; then
            if [ -n "$VIRTUAL_ENV" ]; then
                echo "Deactivating $venv"
                deactivate
            fi
            echo "Deleting $venv"
            rm -r -f $venv
        fi
    fi
    if [ ! -d $venv ]; then
        echo "Creating $venv"
        python -m venv $venv
    fi
    if [ -z "$VIRTUAL_ENV" ]; then
        echo "Activating $venv"
        source $venv/bin/activate
    fi
fi

drkcraft_egg_link="$venv/lib/site-package/drkcraft-py.egg-link"

if [ ! -f $drkcraft_egg_link ]; then
    build_drkcraft="true"
else
    build_drkcraft="false"
fi

if [ "$update" == "true" ]; then
    install_packages="true"
elif [ "$no_venv" == "true" ]; then
    install_packages="true"
elif [ "$build_drkcraft" == "false" ]; then
    install_packages="true"
else
    install_packages="false"
fi

if [ "$build_drkcraft" == "true" ]; then
    echo "Building drkcraft-py package"
    pip install -e python
fi
if [ "$install_packages" == "true" ]; then
    echo "Installing drkcraft-py packages"
    pip install --upgrade -r python/requirements.txt

    echo "Installing launcher packages"
    pip install --upgrade -r launcher/requirements.txt -r launcher/require-types.txt

    echo "Installing std_usage packages"
    pip install --upgrade -r tools/std_usage/requirements.txt
fi
