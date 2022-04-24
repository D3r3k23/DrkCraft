from typing import *
import subprocess
import os.path
import sys

from . import config
from .config import Option, BuildConfig
from .build import build

OPTIONS = (
    Option.build_config,
    Option.en_dev_mode,
    Option.en_trace_log
)

def main(argv: list[str]=sys.argv) -> Optional[int]:
    args = config.parse_args(argv, OPTIONS, 'Runs DrkCraft')

    return run(args.build_config, argv[1:], args.en_dev_mode, args.en_trace_log)

def run(build_config: BuildConfig, args: list[str], en_dev_mode: bool=False, en_trace_log: bool=False) -> Optional[int]:
    exe = config.get_exe(build_config)

    if not os.path.isfile(exe):
        print('Executable "{exe}" not found: building DrkCraft')
        if not build(build_config, True, en_dev_mode):
            return 1

    drkcraft_cmd = [ exe ] + args
    print('>>> ' + ' '.join(drkcraft_cmd))
    try:
        subprocess.run(drkcraft_cmd)
    except subprocess.CalledProcessError:
        return 1

if __name__ == '__main__':
    sys.exit(main(sys.argv))
