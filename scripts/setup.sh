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

if [ "$update" == "true" ]; then
    install_packages="true"
elif [ "$no_venv" == "true" ]; then
    install_packages="true"
elif [ ! -f "$venv/lib/site-packages/drkcraft-py.egg-link" ]; then
    install_packages="true"
else
    install_packages="false"
fi

if [ "$install_packages" == "true" ]; then
    echo "Building drkcraft-py package"
    pip install -e python

    echo "Installing drkcraft-py packages"
    pip install --upgrade -r python/requirements.txt

    echo "Installing launcher packages"
    pip install --upgrade -r launcher/requirements.txt
fi
