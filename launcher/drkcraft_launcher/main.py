from typing import *
import argparse
import logging

import dearpygui.dearpygui as dpg

from . import __version__
from . import launcher

APP_NAME = 'DrkCraft Launcher'

WINDOW_SIZE = WINDOW_WIDTH, WINDOW_HEIGHT = ( 960, 720 )

def main():
    parser = argparse.ArgumentParser(prog=APP_NAME)
    parser.add_argument('-V', '--version', action='version', version=__version__)
    parser.add_argument('--debug', action='store_true', help='Enables debug logging')
    args = parser.parse_args()

    launcher.setup_logger(args.debug)

    logging.info('Setting up DearPyGui')
    dpg.create_context()

    logging.debug('Creating DPG viewport')
    dpg.create_viewport(title=APP_NAME, width=WINDOW_WIDTH, height=WINDOW_HEIGHT)
    dpg.setup_dearpygui()

    launcher.run(APP_NAME)

    logging.info('Closing DPG')
    dpg.destroy_context()

if __name__ == '__main__':
    main()
