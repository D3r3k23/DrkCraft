from typing import *
import subprocess
import sys
import os

import drkcraft
from . import config
from .config import Option, BuildConfig

OPTIONS = (
    Option.build_config,
    Option.en_profiling,
    Option.en_dev_mode
)

def main(argv: list[str]=sys.argv) -> Optional[int]:
    args = config.parse_args(argv, OPTIONS, 'Builds DrkCraft')
    timer = drkcraft.Timer()

    if not build(args.build_config, args.en_profiling, args.en_dev_mode):
        return 1

    exe = config.get_exe(args.build_config)
    print(f'Build time: {timer}')
    print(f'Executable size: {drkcraft.get_file_size_mb(exe):.2f}MB')

def build(build_config: BuildConfig, en_profiling: bool=False, en_dev_mode: bool=False) -> bool:
    build_dir = config.get_build_dir()

    if not os.path.isdir(build_dir):
        os.mkdir(build_dir)

    print('============ Configuring CMake ============')
    cmake_configure = [ 'cmake',
        '-S', '.',
        '-B', build_dir,
        '-D', f'DRK_DIST={"ON" if build_config == BuildConfig.Dist else "OFF"}',
        '-D', f'CMAKE_BUILD_TYPE={build_config.to_cmake_build_type()}',
        '-D', f'DRK_EN_PROFILING={en_profiling}',
        '-D', f'DRK_EN_DEV_MODE={en_dev_mode}'
    ]
    try:
        subprocess.run(cmake_configure, check=True)
    except subprocess.CalledProcessError:
        print('============ CMake configuration failed ============')
        return False

    print('============ Building DrkCraft ============')
    cmake_build = [ 'cmake',
        '--build', build_dir,
        '--config', build_config.to_cmake_build_type()
    ]
    try:
        subprocess.run(cmake_build, check=True)
    except subprocess.CalledProcessError:
        print('============ DrkCraft build failed ============')
        return False
    else:
        print('============ DrkCraft build completed ============')
        return True

if __name__ == '__main__':
    sys.exit(main(sys.argv))
