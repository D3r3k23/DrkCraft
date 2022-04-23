from typing import *
import logging
import os
from datetime import datetime

import dearpygui.dearpygui as dpg
import semver

import drkcraft
from drkcraft.tools import clean_logs

LOG_DIR = 'logs'

def run(title: str='DrkCraft Launcher', debug: bool=False):
    logging.info('Starting DrkCraft Launcher')

    logging.debug('Creating DPG window')
    with dpg.window(tag='Primary Window'):
        dpg.add_text("Welcome to DrkCraft")
        dpg.add_button(label="Start", callback=start_callback)
        dpg.add_input_text(label="string")
        dpg.add_slider_float(label="float")

    logging.info('Starting DPG')
    dpg.set_primary_window('Primary Window', True)
    dpg.show_viewport()
    dpg.start_dearpygui()

def start_callback():
    print("Start")

def setup_logger(debug: bool=True):
    if os.path.isdir(LOG_DIR):
        clean_logs.clean_logs(LOG_DIR, 6)
    else:
        os.makedirs(LOG_DIR)

    log_name = f'DrkCraft_Launcher_{datetime.now().strftime("%Y-%m-%d_%H.%M.%S")}.log'

    file_level = logging.INFO if debug else logging.WARN
    console_level = logging.DEBUG if debug else None
    log_level = console_level if console_level is not None else file_level

    file_handler = logging.FileHandler(os.path.join(LOG_DIR, log_name)).setLevel(file_level)
    console_handler = logging.StreamHandler().setLevel(console_level) if console_level is not None else None

    handlers = [ file_handler ] + [ console_handler ] if console_handler is not None else []

    logging.basicConfig(level=log_level, handlers=handlers)

if __name__ == '__main__':
    run()
