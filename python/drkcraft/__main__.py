from typing import *
import argparse
import sys

import drkcraft

from .build import build
from .build import run
from .build import clean
from .build import package

from .tools import profile
from .tools import clean_logs
from .tools import gen_block_texture_atlas

CMD: dict[str, callable] = {
      'build' : build.main,
        'run' : run.main,
      'clean' : clean.main,
    'package' : package.main,

    'profile': profile.main,
    'clean_logs': clean_logs.main,
    'gen_block_texture_atlas': gen_block_texture_atlas.main
}

HELP_OPTIONS = ( '-h', '--help' )
VERSION_OPTIONS = ( '-V', '--version' )

def main(argv: list[str]=sys.argv) -> Optional[int]:
    prog = argv[0]
    args = argv[1:]
    cmd = args[0] if len(args) > 0 else None

    opt_help:    bool = cmd is not None and any(cmd == o for o in HELP_OPTIONS)
    opt_version: bool = cmd is not None and any(cmd == o for o in VERSION_OPTIONS)

    description = 'Python utilities for DrkCraft'
    usage = f'{prog} [-h] [-V] cmd [-h] [cmd options]'

    parser = argparse.ArgumentParser(prog=prog, description=description, usage=usage, add_help=False)
    parser.add_argument(*HELP_OPTIONS,    action='store_true', help='show this help message and exit')
    parser.add_argument(*VERSION_OPTIONS, action='store_true', help='show DrkCraft version number and exit')

    cmd_choices = list(CMD.keys())
    cmd_help = drkcraft.make_options_str(cmd_choices)

    if opt_help or opt_version:
        cmd_nargs = '?'
        cmd_default = ''
        cmd_choices.append(cmd_default)
        parser.add_argument('cmd', type=str.lower, metavar='cmd', help=cmd_help, choices=cmd_choices, nargs=cmd_nargs, default=cmd_default)
    else:
        parser.add_argument('cmd', type=str.lower, metavar='cmd', help=cmd_help, choices=cmd_choices)

    parser.add_argument('options', nargs='*')
    parsed_args, cmd_args = parser.parse_known_args(args)

    if opt_help:
        parser.print_help()
    elif opt_version:
        print(drkcraft.config.get_version())
    else:
        cmd_name = parsed_args.cmd
        cmd_argv = [ f'{prog} {cmd_name}' ] + cmd_args

        return CMD[cmd_name](cmd_argv)

if __name__ == '__main__':
    argv = [ 'drkcraft' ] + sys.argv[1:]
    sys.exit(main(argv))
