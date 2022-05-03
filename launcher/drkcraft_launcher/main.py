from typing import *
import argparse
import logging
import os
from enum import Enum
from pathlib import Path
from datetime import datetime

import dearpygui.dearpygui as dpg

from drkcraft.tools import clean_logs

from . import *
import installer
import launcher

class Mode(Enum):
    Install = 0
    Launch = 1

def main() -> Optional[int]:
    default_install_dir = installer.get_base_installation_dir()

    parser = argparse.ArgumentParser(prog='DrkCraft Launcher')
    parser.add_argument('-V', '--version', action='version', version=__version__)
    parser.add_argument('-l', '--launch', action='store_true', help='Forces Launch mode')
    parser.add_argument('-d', '--debug', action='store_true', help='Enables debug logging')
    parser.add_argument('-i', '--install', dest='install_dir', help='Overrides base installation directory', default=default_install_dir)
    args = parser.parse_args()

    install_dir = args.install_dir

    in_install_dir = Path(os.getcwd()) == Path(install_dir)
    en_debug = args.debug

    if args.launch or in_install_dir:
        mode = Mode.Launch
        app_name = 'DrkCraft Launcher'
        win_w, win_h = launcher.get_window_size()
    else:
        mode = Mode.Install
        app_name = 'DrkCraft Installer'
        win_w, win_h = installer.get_window_size()

    setup_logger(mode == Mode.Launch, not en_debug)

    logging.info('Setting up DearPyGui')
    dpg.create_context()

    logging.debug('Creating DPG viewport')
    dpg.create_viewport(title=app_name, width=win_w, height=win_h)
    dpg.setup_dearpygui()

    match mode:
        case Mode.Install: installer.run()
        case Mode.Launch: launcher.run()
        case _: logging.error(f'Unknown action: {mode}')

    logging.info('Closing DPG')
    dpg.destroy_context()

def setup_logger(en_file: bool=False, en_console: bool=False):
    if os.path.isdir(LOG_DIR):
        clean_logs.clean_logs(LOG_DIR, 6)
    else:
        os.makedirs(LOG_DIR)

    log_name = f'DrkCraft_Launcher_{datetime.now().strftime("%Y-%m-%d_%H.%M.%S")}.log'

    file_level = logging.WARN
    console_level = logging.DEBUG

    handlers: list[logging.Handler] = []

    if en_file:
        file_handler = logging.FileHandler(os.path.join(LOG_DIR, log_name))
        file_handler.setLevel(file_level)
        handlers.append(file_handler)

    if en_console:
        console_handler = logging.StreamHandler()
        console_handler.setLevel(console_level)
        handlers.append(console_handler)

    if len(handlers) > 0:
        logging.basicConfig(handlers=handlers)

if __name__ == '__main__':
    main()
