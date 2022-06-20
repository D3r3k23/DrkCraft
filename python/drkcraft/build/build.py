from typing import *
import subprocess
import sys

import drkcraft
from . import config
from .config import Option, BuildConfig

OPTIONS = (
    Option.build_config,
    Option.en_profiling,
    Option.en_dev_mode
)

class BuildError(Exception):
    def __init__(self, error_code: int):
        self.error_code = error_code

def main(argv: list[str]=sys.argv) -> Optional[int]:
    args = config.parse_args(argv, OPTIONS, 'Builds DrkCraft')
    timer = drkcraft.Timer()

    try:
        build(args.build_config, args.en_profiling, args.en_dev_mode)
    except BuildError as e:
        return e.error_code
    except KeyboardInterrupt:
        print('============ DrkCraft build canceled ============')
        return 2
    else:
        print('============ DrkCraft build completed ============')

    exe = config.get_exe(args.build_config)
    print(f'Build time: {timer}')
    print(f'Executable size: {drkcraft.get_file_size_mb(exe):.2f}MB')

def build(build_config: BuildConfig, en_profiling: bool=False, en_dev_mode: bool=False):
    build_dir = config.get_build_dir()

    if not build_dir.is_dir():
        build_dir.mkdir()

    print('============ Configuring CMake ============')
    cmake_configure = [ 'cmake',
        '-S', '.',
        '-B', str(build_dir),
        '-D', f'CMAKE_BUILD_TYPE={build_config.to_cmake_build_type()}',
        '-D', f'DRK_DIST={build_config == BuildConfig.Dist}',
        '-D', f'DRK_EN_PROFILING={en_profiling}',
        '-D', f'DRK_EN_DEV_MODE={en_dev_mode}'
    ]
    print('>>>', ' '.join(cmake_configure))
    try:
        subprocess.run(cmake_configure, check=True)
    except subprocess.CalledProcessError as e:
        print('============ DrkCraft CMake configuration failed ============')
        raise BuildError(e.returncode)

    print('============ Building DrkCraft ============')
    cmake_build = [ 'cmake',
        '--build', str(build_dir),
        '--config', build_config.to_cmake_build_type()
    ]
    print('>>>', ' '.join(cmake_build))
    try:
        subprocess.run(cmake_build, check=True)
    except subprocess.CalledProcessError as e:
        print('============ DrkCraft build failed ============')
        raise BuildError(e.returncode)

if __name__ == '__main__':
    sys.exit(main(sys.argv))
