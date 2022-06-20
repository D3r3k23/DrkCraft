from typing import *
import logging

import dearpygui.dearpygui as dpg
import semver

from config import *

window_size = (window_width, window_height) = (960, 720)

def create_gui():
    logging.info('Starting DrkCraft Launcher')
    dpg.add_text("Welcome to DrkCraft")

    def start_callback():
        print("Start")
    dpg.add_button(label="Start", callback=start_callback)

    dpg.add_input_text(label="string")

    dpg.add_slider_float(label="float")

