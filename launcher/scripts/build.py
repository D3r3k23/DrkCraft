from typing import *
import sys
import os
import shutil
import platform
from pathlib import Path

import PyInstaller.__main__

PLATFORM = platform.system()

def main() -> Optional[int]:
    if not 'VIRTUAL_ENV' in os.environ:
        print('Error: Activate .venv')
        return 2

    if Path(os.getcwd()).name != 'launcher':
        if os.path.isdir('launcher'):
            os.chdir('launcher')
        else:
            return 2

    lib_path = os.path.join(os.environ['VIRTUAL_ENV'], 'Lib' if PLATFORM == 'Windows' else 'lib', 'site-packages')

    PyInstaller.__main__.run([
        os.path.join('drkcraft_launcher', 'main.py'),
        '--name', 'DrkCraft Launcher',
        '--onefile',
        '--paths', lib_path,
        '--specpath', 'install',
        '--workpath', os.path.join('install', 'build'),
        '--distpath', os.path.join('install', 'dist'),
        '--noconfirm'
    ])

    exe_name = 'DrkCraft Launcher' + '.exe' if PLATFORM == 'Windows' else ''
    exe_src = os.path.join('install', 'dist', exe_name)

    bin_dir = os.path.join('install', 'bin')
    bin_exe = os.path.join(bin_dir, exe_name)

    if not os.path.isdir(bin_dir):
        os.mkdir(bin_dir)

    if os.path.isfile(bin_exe):
        os.remove(bin_exe)

    shutil.copy(exe_src, bin_exe)

if __name__ == '__main__':
    sys.exit(main())
