from typing import *
import logging
import os.path

from config import *
import _license as LICENSE

def run():
    ...

def get_window_size() -> tuple[int, int]:
    return ( 600, 480 )

def get_base_installation_dir() -> str:
    match PLATFORM:
        case 'Windows': return os.path.expanduser('~\\AppData\\Local\\Programs\\DrkCraft')
        case 'Linux': return os.path.expanduser('~/DrkCraft')
        case _:
            logging.error(f'Unknown platform: {PLATFORM}')
            return ''

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
