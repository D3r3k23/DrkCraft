from typing import *
from pathlib import Path
import argparse
import logging
import sys
from enum import Enum, auto
from datetime import datetime

import dearpygui.dearpygui as dpg

from config import *
import installer
import launcher

FONT_PATH = Path('Launcher/resources/fonts/Kanit-Medium.ttf')

class Mode(Enum):
    Install = auto()
    Launch = auto()
    UpdateLauncher = auto()

def main(argv: list[str]=sys.argv) -> Optional[int]:
    prog = Path(argv[0]).name
    args = argv[1:]

    default_install_dir = installer.get_base_installation_dir()

    parser = argparse.ArgumentParser(prog=prog)
    parser.add_argument('-V', '--version', action='version', version=VERSION)
    parser.add_argument('-l', '--launch', action='store_true', help='Forces Launch mode')
    parser.add_argument('-d', '--debug', action='store_true', help='Enables debug logging')
    parser.add_argument('-i', '--install', dest='install_dir', type=Path,
        help='Overrides base installation directory', default=default_install_dir)
    parsed_args = parser.parse_args(args)

    en_debug = parsed_args.debug
    force_launch = parsed_args.launch

    install_dir = parsed_args.install_dir
    in_install_dir = install_dir == Path.cwd()

    if force_launch or in_install_dir:
        mode = Mode.Launch
        app_name = 'DrkCraft Launcher'
        win_w, win_h = launcher.window_size
    else:
        mode = Mode.Install
        app_name = 'DrkCraft Installer'
        win_w, win_h = installer.window_size

    setup_logger(mode == Mode.Launch, not en_debug)

    logging.info('Setting up DearPyGui')
    dpg.create_context()

    logging.debug('Creating DPG viewport')
    dpg.create_viewport(title=app_name, width=win_w, height=win_h)
    dpg.setup_dearpygui()

    logging.debug('Setting up font')
    setup_font(FONT_PATH)

    logging.debug('Creating DPG window')
    with dpg.window(tag='Primary Window'):
        match mode:
            case Mode.UpdateLauncher: installer.create_gui(update_launcher=True)
            case Mode.Install: installer.create_gui(install_dir)
            case Mode.Launch: launcher.create_gui()
            case _: logging.error(f'Unknown action: {mode}')

    dpg.set_primary_window('Primary Window', True)

    logging.info('Running DPG')
    dpg.show_viewport()
    dpg.start_dearpygui()

    logging.info('Closing DPG')
    dpg.destroy_context()

def setup_font(font: Path):
    with dpg.font_registry():
        with dpg.font(font, 20, tag="DrkCraft Font"):
            dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)
        dpg.bind_font(dpg.last_container())

def setup_logger(en_file: bool=False, en_console: bool=False):
    if LOG_DIR.is_dir():
        pass # clean_logs.clean_logs(LOG_DIR, 6)
    else:
        LOG_DIR.mkdir(parents=True)

    log_name = f'DrkCraft_Launcher_{datetime.now().strftime("%Y-%m-%d_%H.%M.%S")}.log'

    file_level = logging.WARN
    console_level = logging.DEBUG

    handlers: list[logging.Handler] = []

    if en_file:
        file_handler = logging.FileHandler(LOG_DIR / log_name)
        file_handler.setLevel(file_level)
        handlers.append(file_handler)

    if en_console:
        console_handler = logging.StreamHandler()
        console_handler.setLevel(console_level)
        handlers.append(console_handler)

    if len(handlers) > 0:
        logging.basicConfig(handlers=handlers)

if __name__ == '__main__':
    sys.exit(main())
