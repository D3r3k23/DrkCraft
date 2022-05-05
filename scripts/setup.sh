if [ "$1" == "--clean" ] || [ "$2" == "--clean" ]; then
    clean="true"
else
    clean="false"
fi
if [ "$1" == "--no-venv" ] || [ "$2" == "--no-venv" ]; then
    no_venv="true"
else
    no_venv="false"
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
    else
        echo "Creating $venv"
        python -m venv $venv
    fi
    if [ -z "$VIRTUAL_ENV" ]; then
        echo "Activating $venv"
        source $venv/bin/activate
    fi
fi

if [ "$no_venv" == "true" ]; then
    install_packages="true"
elif ! pip list | grep drkcraft-py > /dev/null; then
    install_packages="true"
else
    install_packages="false"
fi

if [ "$install_packages" == "true" ]; then
    echo "Building drkcraft-py package"
    pip install -e python

    echo "Installing drkcraft-py packages"
    pip install -r python/requirements.txt

    echo "Installing launcher packages"
    pip install -r launcher/requirements.txt
fi

# X11 development package is required to compile GLFW
# - on Debian (Ubuntu & Linux Mint)
sudo apt install xorg-dev
