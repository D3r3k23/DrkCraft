from typing import *
from pathlib import Path
import logging

import dearpygui.dearpygui as dpg
import semver

window_size = (window_width, window_height) = (960, 720)

FONT_PATH = Path('Launcher/resources/fonts/Kanit-Medium.ttf')

def create_gui():
    logging.info('Starting DrkCraft Launcher')

    drkcraft_dir = Path.cwd()

    setup_font(FONT_PATH)

    dpg.add_text("Welcome to DrkCraft")

    def start_callback(sender, app_data, user_data):
        print("Start")

    dpg.add_button(label="Start", callback=start_callback)

    dpg.add_input_text(label="string")

    dpg.add_slider_float(label="float")

def setup_font(font: Path):
    with dpg.font_registry():
        with dpg.font(font, 20, tag="DrkCraft Font"):
            dpg.add_font_range_hint(dpg.mvFontRangeHint_Default)
        dpg.bind_font(dpg.last_container())

