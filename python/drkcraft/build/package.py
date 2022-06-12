from typing import *
import shutil
import sys
import os
import pathlib

from ruamel.yaml import YAML
yaml = YAML(typ='safe')

import strip_markdown

import drkcraft
from . import config
from .config import Option, BuildConfig
from .build import build
from .clean import clean

OPTIONS = (
    Option.build_config,
    Option.en_profiling,
    Option.en_dev_mode,
    Option.skip_build
)

def main(argv: list[str]=sys.argv) -> Optional[int]:
    args = config.parse_args(argv, OPTIONS, 'Builds DrkCraft package')
    timer = drkcraft.Timer()

    package_path = package(args.build_config, args.en_profiling, args.en_dev_mode, not args.skip_build)
    if package_path is None:
        return 1

    print(f'Package build time: {timer}')
    print(f'Package size: {drkcraft.get_file_size_mb(package_path):.2f}MB')

PACKAGE_DIR = os.path.join('packages', 'DrkCraft')

PLATFORM = drkcraft.get_platform()
VERSION = config.get_version()

def package(build_config: BuildConfig, en_profiling: bool=False, en_dev_mode: bool=False, en_build: bool=True) -> Optional[str]:
    package_name = f'DrkCraft-v{VERSION}-{PLATFORM}-{build_config}'
    package_zip = os.path.join('packages', package_name + '.zip')

    print(f'Preparing package: {package_name}')

    exe = config.get_exe(build_config)
    if en_build:
        clean(True)
        if not build(build_config, en_profiling, en_dev_mode):
            return None
    elif not os.path.isfile(exe):
        print('Package error: DrkCraft not built')
        return None

    if os.path.isdir(PACKAGE_DIR):
        shutil.rmtree(PACKAGE_DIR)
    os.mkdir(PACKAGE_DIR)

    print(f'Building package: {package_name}')

    print('Copying executable')
    shutil.copy(exe, PACKAGE_DIR)

    print('Copying assets')
    unnecessary_assets = (
        os.path.join('assets', 'images', 'textures', 'blocks'),
        os.path.join('assets', 'images', 'textures', 'dev')
    )
    ignore_assets = shutil.ignore_patterns(*unnecessary_assets) if build_config == BuildConfig.Dist else None
    shutil.copytree('assets', os.path.join(PACKAGE_DIR, 'assets'), ignore=ignore_assets)

    print('Creating config directory')
    shutil.copytree(os.path.join('config', 'default'), os.path.join(PACKAGE_DIR, 'config'))

    print('Creating data directory')
    data_dir = os.path.join(PACKAGE_DIR, 'data')
    os.mkdir(data_dir)

    if en_profiling:
        print('Creating profile directory')
        os.mkdir(os.path.join(data_dir, 'profile'))

        print('Copying profile README')
        strip_markdown.strip_markdown_file(os.path.join('data', 'profile', 'README.md'), os.path.join(data_dir, 'profile'))

    if build_config != BuildConfig.Dist:
        print('Creating tools directory')
        tools_dir = os.path.join(PACKAGE_DIR, 'tools')
        os.mkdir(tools_dir)

        tools_src_dir = os.path.join('python', 'drkcraft', 'tools')
        shutil.copy(os.path.join(tools_src_dir, '__init__.py'), tools_dir)
        shutil.copy(os.path.join(tools_src_dir, 'requirements.txt'), os.path.join(tools_dir, 'requirements.txt'))

        if en_profiling:
            shutil.copy(os.path.join(tools_src_dir, 'profile.py'), tools_dir)
        shutil.copy(os.path.join(tools_src_dir, 'clean_logs.py'), tools_dir)
        shutil.copy(os.path.join(tools_src_dir, 'gen_block_texture_atlas.py'), tools_dir)
        shutil.copy(os.path.join('data', 'blocks.yaml'), data_dir)

        print('Writing tools README')
        write_tools_readme_file(os.path.join(tools_dir, 'README.txt'), en_profiling)

    print('Creating about directory')
    about_dir = os.path.join(PACKAGE_DIR, 'about')
    os.mkdir(about_dir)
    shutil.copy('VERSION.txt', about_dir)

    print('Writing build file')
    write_build_file(os.path.join(about_dir, 'build.txt'), VERSION, PLATFORM, str(build_config), en_profiling, en_dev_mode)

    print('Writing README')
    write_readme_file(os.path.join(about_dir, 'README.txt'), VERSION, pathlib.Path(exe).name,
        en_dev_mode, build_config == BuildConfig.Debug)

    print('Writing LICENSE')
    write_license_file(os.path.join(about_dir, 'LICENSE.txt'), 'LICENSE.txt', os.path.join('lib', 'LICENSE.txt'))

    print(f'Creating archive: {package_zip}')
    if os.path.isfile(package_zip):
        os.remove(package_zip)
    shutil.make_archive(str(pathlib.Path(package_zip).with_suffix('')), 'zip', 'packages', 'DrkCraft')

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

def write_readme_file(filename: str, version: str, exe_name: str, dev: bool=False, logging: bool=False):
    manual_url: str = f'https://github.com/D3r3k23/DrkCraft/tree/v{version}/doc/manual.md'
    shell_char = '>' if PLATFORM == 'Windows' else '$'

    lines = [
        README_TITLE,
        '',
        f'To play, run the file {exe_name} in the root DrkCraft directory',
        f'Command line usage: {os.path.join("DrkCraft", "")} {shell_char} {os.path.join(".", exe_name)} [--dev] [--trace]',
        '',
        'Options:',
        '  --dev   Enable Dev mode' if dev else '',
        '  --trace Enable console trace logging' if logging else '',
        '',
        f'For help, see the manual: {manual_url}',
        '',
        'License information available in DrkCraft/about/LICENSE.txt and in DrkCraft/assets'
    ]

    with open(filename, 'w') as f:
        f.writelines(f'{line}\n' for line in lines)

def write_build_file(filename: str, version: str, platform: str, build_config: str, en_profiling: bool=False, en_dev_mode: bool=False):
    with open(filename, 'w') as f:
        f.write(f'Version: v{version}\n')
        f.write(f'Platform: {platform}\n')
        f.write(f'Config: {build_config}\n')
        if build_config != BuildConfig.Dist:
            f.write(f'Profiling: {bool_to_en_dis(en_profiling)}\n')
            f.write(f'Dev mode: {bool_to_en_dis(en_dev_mode)}\n')

def write_license_file(filename: str, drkcraft_license_filename: str, lib_license_filename: str):
    lines: list[str] = []

    with open(drkcraft_license_filename, 'r') as f:
        drkcraft_license = f.read()

    lines += [
        '========== DrkCraft ============================================================',
        '',
        drkcraft_license,
        '',
        '================================================================================'
    ]

    with open(lib_license_filename, 'r') as f:
        lib_licenses = yaml.load(f)

    for lib, license in lib_licenses.items():
        lines += [
            '',
            f'{"=" * 10 + f" {lib} " : =>80}',
            '',
            license,
            '',
            '=' * 80
        ]

    with open(filename, 'w') as f:
        f.writelines(f'{line}\n' for line in lines)

def write_tools_readme_file(filename: str, en_profiling: bool=False):
    lines = [
        'TOOLS',
        '',
        '- Install dependencies using `python -m pip install -r requirements.txt`',
        ''
    ]
    if en_profiling:
        lines += [
            '****** profile.py ******',
            f'usage: python {os.path.join("tools", "profile.py")} [-h] [profile]',
            'Analyzes profiler results',
            f'- Default profile: {os.path.join("data", "profile", "results.json")}',
            '',
        ]
    lines += [
        '****** clean_logs.py ******',
        f'usage: python {os.path.join("tools", "clean_logs.py")} [-h] [log_dir] [--max_age]',
        '- Cleans log directory',
        ''
    ]
    lines += [
        '****** gen_block_texture_atlas.py ******',
        f'usage: python {os.path.join("tools", "gen_block_texture_atlas.py")} [-h] [atlas] [--blocks] [--textures]',
        '- Generates textures atlas image for all blocks',
    ]

    with open(filename, 'w') as f:
        f.writelines(f'{line}\n' for line in lines)

def bool_to_en_dis(enabled: bool) -> str:
    return 'Enabled' if enabled else 'Disabled'

if __name__ == '__main__':
    sys.exit(main(sys.argv))
