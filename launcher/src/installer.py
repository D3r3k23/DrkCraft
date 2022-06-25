from typing import *
from pathlib import Path
import logging

window_size = (window_width, window_height) = (600, 480)

def create_gui(install_dir: Path, update_launcher: bool=False):
    logging.info('Starting DrkCraft Installer')

    drkcraft_dir = install_dir / 'DrkCraft'
