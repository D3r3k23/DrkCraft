from typing import *
from pathlib import Path
import logging

from config import *

window_size = (window_width, window_height) = (600, 480)

def create_gui(install_dir: Optional[Path]=None, update_launcher: bool=False):
    logging.info('Starting DrkCraft Installer')

    if install_dir is None:
        install_dir = get_base_installation_dir()

def get_base_installation_dir() -> Path:
    match PLATFORM:
        case 'Windows': return Path.home() / 'AppData' / 'Local' / 'Programs' / 'DrkCraft'
        case 'Linux': return Path.home() / 'DrkCraft'
        case _:
            logging.error(f'Unknown platform: {PLATFORM}')
            return Path()
