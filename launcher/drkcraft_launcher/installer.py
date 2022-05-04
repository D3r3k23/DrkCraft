from typing import *
import os.path

from drkcraft_launcher import *

def run():
    ...

def get_window_size() -> tuple[int, int]:
    return ( 600, 480 )

def get_base_installation_dir() -> str:
    return os.path.join(os.path.expanduser('~'), '.drkcraft')
