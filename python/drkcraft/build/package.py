from typing import *
from pathlib import Path
import shutil
import sys
import os.path
# import tomllib
import toml # type: ignore

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

    package_path = package(args.build_config, args.en_profiling, args.en_dev_mode, args.skip_build)
    if package_path is None:
        return 1

    print(f'Package build time: {timer}')
    print(f'Package size: {drkcraft.get_file_size_mb(package_path):.2f}MB')

PACKAGE_DIR = Path('packages/DrkCraft')

PLATFORM = drkcraft.get_platform()
VERSION = config.get_version()

def package(build_config: BuildConfig, en_profiling: bool=False, en_dev_mode: bool=False, skip_build: bool=False) -> Optional[Path]:
    package_name = f'DrkCraft-v{VERSION}-{PLATFORM}-{build_config}'
    package_zip = Path('packages') / f'{package_name}.zip'

    print(f'Preparing package: {package_name}')

    exe = config.get_exe(build_config)
    if not skip_build:
        clean(True)
        if not build(build_config, en_profiling, en_dev_mode):
            return None
    elif not exe.is_file():
        print('Package error: DrkCraft not built')
        return None

    if PACKAGE_DIR.is_dir():
        shutil.rmtree(PACKAGE_DIR)
    PACKAGE_DIR.mkdir(parents=True)

    print(f'Building package: {package_name}')

    print('Copying executable')
    shutil.copy(exe, PACKAGE_DIR)

    print('Copying assets')
    unnecessary_assets = (
        Path('assets/images/textures/blocks'),
        Path('assets/images/textures/dev')
    )
    ignore_assets = shutil.ignore_patterns(*unnecessary_assets) if build_config == BuildConfig.Dist else None
    shutil.copytree('assets', PACKAGE_DIR / 'assets', ignore=ignore_assets)

    print('Creating config directory')
    Path(PACKAGE_DIR / 'config').mkdir()

    print('Creating data directory')
    data_dir = PACKAGE_DIR / 'data'
    data_dir.mkdir()

    if build_config != BuildConfig.Dist:
        shutil.copy(Path('data/blocks.yaml'), data_dir)

    if en_profiling:
        print('Creating profile directory')
        Path(data_dir / 'profile').mkdir()

        print('Copying profile README')
        strip_markdown.strip_markdown_file(Path('data/profile/README.md'), data_dir / 'profile')

    if build_config != BuildConfig.Dist:
        print('Creating tools directory')
        tools_dir = PACKAGE_DIR / 'tools'
        tools_dir.mkdir()

        tools_src_dir = Path('python/drkcraft/tools')
        shutil.copy(tools_src_dir / 'requirements.txt', tools_dir / 'requirements.txt')

        if en_profiling:
            shutil.copy(tools_src_dir / 'profile.py', tools_dir)
        shutil.copy(tools_src_dir / 'clean_logs.py', tools_dir)
        shutil.copy(tools_src_dir / 'gen_block_texture_atlas.py', tools_dir)

        print('Writing tools README')
        write_tools_readme_file(tools_dir / 'README.txt', en_profiling)

    print('Creating about directory')
    about_dir = PACKAGE_DIR / 'about'
    about_dir.mkdir()
    shutil.copy('VERSION.txt', about_dir)

    print('Writing build file')
    write_build_file(about_dir / 'build.txt', VERSION, PLATFORM, str(build_config), en_profiling, en_dev_mode)

    print('Writing README')
    write_readme_file(about_dir / 'README.txt', VERSION, exe.name, en_dev_mode, build_config == BuildConfig.Debug)

    print('Writing LICENSE')
    write_license_file(about_dir / 'LICENSE.txt', Path('LICENSE.txt'), Path('lib') / 'LICENSE.txt')

    print(f'Creating archive: {package_zip}')
    if package_zip.is_file():
        package_zip.unlink()
    shutil.make_archive(str(package_zip.with_suffix('')), 'zip', 'packages', 'DrkCraft')

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

def write_readme_file(filename: Path, version: str, exe_name: str, dev: bool=False, logging: bool=False):
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

def write_build_file(filename: Path, version: str, platform: str, build_config: str, en_profiling: bool=False, en_dev_mode: bool=False):
    with open(filename, 'w') as f:
        f.write(f'Version: {version}\n')
        f.write(f'Platform: {platform}\n')
        f.write(f'Config: {build_config}\n')
        if build_config != BuildConfig.Dist:
            f.write(f'Profiling: {bool_to_en_dis(en_profiling)}\n')
            f.write(f'Dev mode: {bool_to_en_dis(en_dev_mode)}\n')

def write_license_file(filename: Path, drkcraft_license_filename: Path, lib_license_filename: Path):
    with open(drkcraft_license_filename, 'r') as f:
        drkcraft_license = f.read()

    with open(lib_license_filename, 'r') as f:
        lib_licenses = toml.loads(f.read())
    # with open(lib_license_filename, 'rb') as f:
        # lib_licenses = tomllib.load(f)

    def make_license(title, text) -> str:
        lines = [
            make_header(title),
            text,
            make_footer()
        ]
        return '\n'.join(lines)

    # start_length                   ... total_length: vvv
    ############## <TITLE> ###############################
    def make_header(title: str, start_length=10, total_length=80) -> str:
        base = f"{'=' * start_length} {title} "
        return f"{base:=<{total_length}}\n"

    ######################################################
    def make_footer(length=80) -> str:
        return f"{'=' * length}\n"

    licenses: list[str] = [
        make_license('DrkCraft', drkcraft_license)
    ] + [
        make_license(title, text) for title, text in lib_licenses.items()
    ]

    with open(filename, 'w') as f:
        f.writelines('\n'.join(licenses))

def write_tools_readme_file(filename: Path, en_profiling: bool=False):
    lines = [
        'TOOLS',
        '',
        '- Install dependencies using `python -m pip install -r requirements.txt`',
        ''
    ]
    if en_profiling:
        lines += [
            '****** profile.py ******',
            f'usage: python {Path("tools/profile.py")} [-h] [profile]',
            'Analyzes profiler results',
            f'- Default profile: {Path("data/profile", "results.json")}',
            '',
        ]
    lines += [
        '****** clean_logs.py ******',
        f'usage: python {Path("tools/clean_logs.py")} [-h] [log_dir] [--max_age]',
        '- Cleans log directory',
        ''
    ]
    lines += [
        '****** gen_block_texture_atlas.py ******',
        f'usage: python {Path("tools/gen_block_texture_atlas.py")} [-h] [atlas] [--blocks] [--textures]',
        '- Generates textures atlas image for all blocks',
    ]

    with open(filename, 'w') as f:
        f.writelines(f'{line}\n' for line in lines)

def bool_to_en_dis(enabled: bool) -> str:
    return 'Enabled' if enabled else 'Disabled'

if __name__ == '__main__':
    sys.exit(main(sys.argv))
