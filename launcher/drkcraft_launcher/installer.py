from typing import *
import os.path

from . import *

def run():
    ...

def get_window_size() -> tuple[int, int]:
    return ( 600, 480 )

def get_base_installation_dir() -> Optional[str]:
    match PLATFORM:
        case 'Window' : app_dir = os.path.expandvars('${LOCALAPPDATA}\\Programs')
        case 'Linux'  : app_dir = os.path.expandvars('${HOME}')
        case _: return None

    return os.path.join(app_dir, 'DrkCraft')
