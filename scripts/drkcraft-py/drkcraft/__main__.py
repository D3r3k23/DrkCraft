from typing import *
import argparse
import sys

import drkcraft
from . import build
from . import run
from . import clean
from . import package

CMD = {
      'build' : build.main,
        'run' : run.main,
      'clean' : clean.main,
    'package' : package.main
}

def main(argv: list[str]=sys.argv) -> Optional[int]:
    sys.argv[0] = 'drkcraft'

    help: bool = any(h in argv for h in { '-h', '--help' })

    parser = argparse.ArgumentParser(description='Python utilities for DrkCraft', add_help=False)
    parser.add_argument('-h', '--help', action='store_true', help='show this help message and exit')
    parser.add_argument('-V', '--version', action='version', version=drkcraft.config.get_version())
    parser.add_argument('cmd', type=str, metavar='cmd', help='[build|run|clean|package]',
        choices=(list(CMD.keys()) + ([''] if help else [])), nargs=('?' if help else '+'), default='')
    args = parser.parse_args(argv[1:])

    cmd_name = args.cmd

    if help:
        if args.cmd == '':
            parser.print_help()
            return 0
        else:
            cmd_args = [ '-h' ]
    else:
        cmd_args = sys.argv[2:]

    return CMD[cmd_name](cmd_args)

if __name__ == '__main__':
    sys.exit(main(sys.argv))
