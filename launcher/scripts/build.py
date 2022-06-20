from typing import *
from pathlib import Path
import subprocess
import sys
import os.path
import platform
import shutil

PLATFORM = platform.system()

VENV = Path('.venv.')
BUILD_DIR = Path('build')

MAIN_SRC = Path('src/main.py')
EXE_NAME = 'drkcraft-launcher'

def main() -> Optional[int]:
    print('======== Building DrkCraft Launcher ========')

    install_dir = BUILD_DIR / 'install'
    if not install_dir.is_dir():
        install_dir.mkdir(parents=True)

    work_dir = install_dir / 'build'
    bin_dir = install_dir / 'bin'

    exe_path = bin_dir / Path(EXE_NAME + exe_ext())

    lib_path = VENV / ('Lib' if PLATFORM == 'Windows' else 'lib') / 'site-packages'

    build_cmd = [ 'pyinstaller',
        str(MAIN_SRC),
        '--name', EXE_NAME,
        '--onefile',
        '--paths', str(lib_path),
        '--specpath', str(install_dir),
        '--workpath', str(work_dir),
        '--distpath', str(bin_dir),
        '--noconfirm'
    ]
    print('>>>', ' '.join(build_cmd))
    try:
        subprocess.run(build_cmd, check=True)
    except subprocess.CalledProcessError as e:
        print('======== PyInstaller build failed ========')
        return e.returncode
    else:
        print('======== PyInstaller build completed ========')

    exe_size = os.path.getsize(exe_path)
    print(f'Executable size: {exe_size / 10**6:.2f}MB')

    build_package(exe_path)

def build_package(exe: Path):
    print('======== Building DrkCraft Launcher package ========')

    package_dir = BUILD_DIR / 'package'
    if package_dir.is_dir():
        shutil.rmtree(package_dir)
    package_dir.mkdir()

    installer_path = package_dir / Path('drkcraft-installer' + exe_ext())
    shutil.copy(exe, installer_path)

    launcher_dir = package_dir / 'Launcher'
    launcher_dir.mkdir()

    shutil.copy(exe, launcher_dir)
    shutil.copy('LICENSE.txt', launcher_dir)

    shutil.copytree(Path('Launcher') / 'resources', launcher_dir / 'resources')

def exe_ext():
    return '.exe' if PLATFORM == 'Windows' else ''

if __name__ == '__main__':
    sys.exit(main())
