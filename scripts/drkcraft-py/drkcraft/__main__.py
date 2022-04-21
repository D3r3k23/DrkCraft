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
    argv[0] = 'drkcraft'

    parser = argparse.ArgumentParser(description='Python utilities for DrkCraft', add_help=False)
    parser.add_argument('-h', '--help', action='store_true', help='show this help message and exit')
    parser.add_argument('-V', '--version', action='version', version=drkcraft.config.get_version())
    parser.add_argument('cmd', type=str, choices=(list(CMD.keys()) + ['']), metavar='cmd', help='[build|run|clean|package]',
        nargs=('?' if '-h' in sys.argv or '--help' in sys.argv else 1), default='')
    args = parser.parse_args(argv)

    cmd_name = args.cmd[0]

    if args.help:
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
