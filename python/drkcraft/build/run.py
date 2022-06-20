from typing import *
import subprocess
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

    return run(args.build_config, args.en_dev_mode, args.en_trace_log)

def run(build_config: BuildConfig, en_dev_mode: bool=False, en_trace_log: bool=False) -> Optional[int]:
    exe = config.get_exe(build_config)

    if exe.is_file():
        print('Executable "{exe}" not found: building DrkCraft')
        if not build(build_config, True, en_dev_mode):
            return 1

    drkcraft_cmd: list[str] = [ str(exe) ]
    if en_dev_mode: drkcraft_cmd.append('--dev')
    if en_trace_log: drkcraft_cmd.append('--trace')

    print('>>> ' + ' '.join(drkcraft_cmd))
    try:
        subprocess.run(drkcraft_cmd, check=True)
    except subprocess.CalledProcessError as e:
        return e.returncode

if __name__ == '__main__':
    sys.exit(main(sys.argv))
