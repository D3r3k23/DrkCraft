from typing import *
import subprocess
import shutil
import sys
import os
import time
import pathlib

import drkcraft
from drkcraft.config import Option, BuildConfig
from .build import build
from .clean import clean

OPTIONS = (
    Option.build_config,
    Option.en_profiling,
    Option.en_dev_mode
)

def main(argv: list[str]=sys.argv) -> Optional[int]:
    if len(argv) < 1:
        argv.append('dist')

    if sys.argv[0] == 'drkcraft':
        sys.argv[0] += ' package'

    args = drkcraft.config.parse_args(argv, OPTIONS, description='Builds DrkCraft package')

    start = time.perf_counter()

    package_path = package(args.build_config, args.en_profiling, args.en_dev_mode)
    if package_path is None:
        return 1

    end = time.perf_counter()

    elapsed = end - start
    elapsed_min = round(elapsed) // 60
    elapsed_sec = elapsed - elapsed_min * 60
    print(f'Package build time: {elapsed_min}min {elapsed_sec}s')

    bytes = os.path.getsize(package_path)
    megabytes = bytes / 10**6
    print(f'Package size: {megabytes:.1f}MB')

PACKAGE_DIR = os.path.join('packages', 'DrkCraft')

VERSION = drkcraft.config.get_version()
PLATFORM = drkcraft.config.get_platform()

def package(build_config: BuildConfig, en_profiling: bool=False, en_dev_mode: bool=False) -> Optional[str]:
    package_name = f'DrkCraft-v{VERSION}-{PLATFORM}-{build_config}'
    package_zip = os.path.join('packages', package_name, '.zip')

    print(f'Building package: {package_name}')
    clean(True)
    if not build(build_config, en_profiling, en_dev_mode):
        return None
    else:
        exe = drkcraft.config.get_exe(build_config)

    if os.path.isdir(PACKAGE_DIR):
        shutil.rmtree(PACKAGE_DIR)
    os.mkdir(PACKAGE_DIR)

    print('Copying executable')
    shutil.copy(exe, PACKAGE_DIR)

    print('Copying assets')
    shutil.copytree('assets', os.path.join(PACKAGE_DIR, 'assets'))
    if build_config == BuildConfig.Dist:
        shutil.rmtree(os.path.join(PACKAGE_DIR, 'assets', 'images', 'textures', 'blocks'))

    print('Creating config directory')
    shutil.copytree(os.path.join('config', 'default'), os.path.join(PACKAGE_DIR, 'config'))

    print('Creating data directory')
    os.mkdir(os.path.join(PACKAGE_DIR, 'data'))
    os.mkdir(os.path.join(PACKAGE_DIR, 'data', 'logs'))
    os.mkdir(os.path.join(PACKAGE_DIR, 'data', 'profile'))

    if build_config != BuildConfig.Dist:
        print('Creating tools directory')
        os.mkdir(os.path.join(PACKAGE_DIR, 'tools'))

        shutil.copy(os.path.join('scripts', 'gen_block_texture_atlas.py'), os.path.join('package_dir', 'tools'))
        shutil.copy(os.path.join('scripts', 'clean_logs.py'),            os.path.join('package_dir', 'tools'))
        shutil.copy(os.path.join('scripts', 'profile.py'),             os.path.join('package_dir', 'tools'))
        shutil.copy(os.path.join('scripts', 'requirements.txt'),     os.path.join('package_dir', 'tools'))

        shutil.copy(os.path.join('data', 'blocks.yaml'), os.path.join('package_dir', 'data'))

    print('Creating about directory')
    os.mkdir(os.path.join(PACKAGE_DIR, 'about'))

    print('Writing README.txt')
    write_readme_file(os.path.join(PACKAGE_DIR, 'about', 'README.txt'), VERSION, pathlib.Path(exe).name)

    print('Writing build.txt')
    write_build_file(os.path.join(PACKAGE_DIR, 'about', 'build.txt'), VERSION, PLATFORM, str(build_config), en_profiling, en_dev_mode)

    print('Writing LICENSE.txt')
    write_license_file(os.path.join(PACKAGE_DIR, 'about', 'LICENSE.txt'), 'LICENSE.txt', os.path.join('lib', 'LICENSE.txt'))

    print('Creating archive %package_zip%')
    if os.path.isfile(package_zip):
        os.remove(package_zip)
    shutil.make_archive(str(pathlib.Path(package_zip).with_suffix('')), 'zip', '.', 'DrkCraft')

    print(f'Package {package_name} built')
    return package_zip

README_TITLE = (
    R'$$$$$$$\            $$\        $$$$$$\                       $$$$$$\    $$ ''\n'
    R'$$  __$$\           $$ |      $$  __$$\                     $$  __$$\   $$ |''\n'
    R'$$ |  $$ | $$$$$$\  $$ |  $$\ $$ /  \__| $$$$$$\   $$$$$$\  $$ /  \__|$$$$$$ ''\n'
    R'$$ |  $$ |$$  __$$\ $$ | $$  |$$ |      $$  __$$\  \____$$\ $$$$\     \_$$  _|''\n'
    R'$$ |  $$ |$$ |  \__|$$$$$$  / $$ |      $$ |  \__| $$$$$$$ |$$  _|      $$ |   ''\n'
    R'$$ |  $$ |$$ |      $$  _$$<  $$ |  $$\ $$ |      $$  __$$ |$$ |        $$ |$$\ ''\n'
    R'$$$$$$$  |$$ |      $$ | \$$\ \$$$$$$  |$$ |      \$$$$$$$ |$$ |        \$$$$  | ''\n'
    R'\_______/ \__|      \__|  \__| \______/ \__|       \_______|\__|         \____/   ''\n'
)

def write_readme_file(filename: str, version: str, exe_name: str):
    manual_url: str = f'https://github.com/D3r3k23/DrkCraft/tree/v{version}/doc/manual.md'

    lines = [
        README_TITLE,
        '',
        f'To play, run the file {exe_name} in the root DrkCraft directory',
        f'Command line usage: DrkCraft/$: ./{exe_name} [--dev]',
        '',
        'Options:',
        '  --dev   Enable Dev mode (Only available in some builds)',
        '',
        f'For help, see the manual: {manual_url}',
        '',
        'License information available in DrkCraft/about/LICENSE.txt and in DrkCraft/assets'
    ]

    with open(filename, 'w') as f:
        f.writelines(f'{lines}\n' for line in lines)

def write_build_file(filename: str, version: str, platform: str, build_config: str, en_profiling: bool=False, en_dev_mode: bool=False):
    with open(filename, 'w') as f:
        f.write(f'Version: {version}\n')
        f.write(f'Platform: {platform}\n')
        f.write(f'Config: {build_config}\n')
        if build_config != BuildConfig.Dist:
            f.write(f'Profiling: {bool_to_en_dis(en_profiling)}\n')
            f.write(f'Dev mode: {bool_to_en_dis(en_dev_mode)}\n')

def write_license_file(filename: str, drkcraft_license_filename: str, lib_license_filename: str):
    with open(drkcraft_license_filename, 'r') as f:
        drkcraft_license = f.read()

    with open(lib_license_filename, 'r') as f:
        lib_license = f.read()

    with open(filename, 'w') as f:
        f.write('========== DrkCraft ===========================================================\n')
        f.write('\n')
        f.write(drkcraft_license)
        f.write('\n')
        f.write('===============================================================================\n')
        f.write('\n')
        f.write(lib_license)

def bool_to_en_dis(enabled: bool) -> str:
    return 'Enabled' if enabled else 'Disabled'

if __name__ == '__main__':
    sys.exit(main(sys.argv))
