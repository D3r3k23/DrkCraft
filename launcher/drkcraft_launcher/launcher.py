from typing import *
import logging
import os

import dearpygui.dearpygui as dpg
import semver

from drkcraft_launcher import *

def run():
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

def get_window_size() -> tuple[int, int]:
    return ( 960, 720 )

if __name__ == '__main__':
    run()
