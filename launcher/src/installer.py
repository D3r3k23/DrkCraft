from typing import *
import os.path

from config import *
import LICENSE

def run():
    ...

def get_window_size() -> tuple[int, int]:
    return ( 600, 480 )

def get_base_installation_dir() -> str:
    return os.path.join(os.path.expanduser('~'), '.drkcraft')

def write_launcher_license():
    lines = [
        '========== pyimgui =============================================================',
        LICENSE.pyimgui,
        '================================================================================',
        '',
        '========== semver ==============================================================',
        LICENSE.semver,
        '================================================================================',
        ''
    ]
