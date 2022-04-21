from typing import *
import subprocess
import os.path
import sys
import time

import drkcraft
from drkcraft.config import Option, BuildConfig

OPTIONS = (
    Option.build_config,
    Option.en_profiling,
    Option.en_dev_mode
)

def main(argv: list[str]=sys.argv) -> Optional[int]:
    if sys.argv[0] == 'drkcraft':
        sys.argv[0] += ' build'
    else:
        sys.argv[0] = 'drkcraft.build'

    args = drkcraft.config.parse_args(argv, OPTIONS, description='Builds DrkCraft')

    start = time.perf_counter()

    if not build(args.build_config, args.en_profiling, args.en_dev_mode):
        return 1

    end = time.perf_counter()

    elapsed = end - start
    elapsed_min = round(elapsed) // 60
    elapsed_sec = round(elapsed - elapsed_min * 60)
    print(f'Build time: {elapsed_min}min {elapsed_sec}s')

    bytes = os.path.getsize(drkcraft.config.get_exe(args.build_config))
    megabytes = bytes / 10**6
    print(f'Executable size: {megabytes:.1f}MB')

def build(build_config: BuildConfig, en_profiling: bool=False, en_dev_mode: bool=False) -> bool:
    build_dir = drkcraft.config.get_build_dir()

    print('============ Configuring CMake ============')
    cmake_configure = [ 'cmake', '-S', '.', 'B', build_dir ]

    if build_config == BuildConfig.Dist:
        cmake_configure.append('-DDRK_DIST=ON')
    else:
        cmake_configure.append(f'-DCMAKE_BUILD_TYPE={str(build_config)}')
        cmake_configure.append(f'-DDRK_EN_PROFILING={en_profiling}')
        cmake_configure.append(f'-DDRK_EN_DEV_MODE={en_dev_mode}')

    try:
        subprocess.run(cmake_configure, check=True)
    except subprocess.CalledProcessError:
        print('============ CMake configuration failed ============')
        return False

    print('============ Building DrkCraft ============')
    cmake_build = [ 'cmake', '--build', build_dir, '--config', str(build_config) ]

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
