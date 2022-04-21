from typing import *
import subprocess
import os.path
import sys

import drkcraft
from drkcraft.config import Option, BuildConfig
from .build import build

OPTIONS = (
    Option.build_config,
    Option.en_dev_mode,
    Option.en_trace_log
)

def main(argv: list[str]=sys.argv) -> Optional[int]:
    if sys.argv[0] == 'drkcraft':
        sys.argv[0] += ' run'
    else:
        sys.argv[0] = 'drkcraft.run'

    args = drkcraft.config.parse_args(argv, OPTIONS, description='Runs DrkCraft')

    return run(args.build_config, args.en_dev_mode, args.en_trace_log)

def run(build_config: BuildConfig, en_dev_mode: bool=False, en_trace_log: bool=False) -> Optional[int]:
    exe = drkcraft.config.get_exe(build_config)

    if not os.path.isfile(exe):
        print('Executable "{exe}" not found: building DrkCraft')
        if not build(build_config, True, en_dev_mode):
            return 1

    drkcraft_cmd = [ exe ]
    if en_dev_mode: drkcraft_cmd.append('--dev')
    if en_trace_log: drkcraft_cmd.append('--trace')

    try:
        subprocess.run(drkcraft_cmd)
    except subprocess.CalledProcessError:
        return 1

if __name__ == '__main__':
    sys.exit(main(sys.argv))
