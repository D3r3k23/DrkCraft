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

if [ "$no_venv" == "false" ]; then
    venv=".venv"

    if [ -d $venv ]; then
        if [ "$clean" == "true "]; then
            echo "Cleaning virtualenv"
            rm -r -f $venv
            make_venv="true"
        else
            make_venv="false"
        fi
    else
        make_venv="true"
    fi

    if [ "$make_venv" == "true" ]; then
        echo "Creating virtualenv"
        python -m venv $venv
    fi

    echo "Activating virtualenv"
    source $venv/bin/activate
fi

echo "Building drkcraft-py"
mkdir -p scripts/drkcraft-py/build
pip install -q -e scripts/drkcraft-py

echo "Installing dependencies"
pip install --upgrade -r scripts/requirements.txt
pip install --upgrade -r launcher/requirements.txt
