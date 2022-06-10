from typing import *
import argparse
import shutil
import os.path
import sys

from . import config

def main(argv: list[str]=sys.argv) -> Optional[int]:
    prog, args = argv[0], argv[1:]

    description = 'Removes DrkCraft build files'
    usage = f'{prog} [-h] [--all]'

    parser = argparse.ArgumentParser(prog=prog, description=description, usage=usage)
    parser.add_argument('--all', action='store_true', help='Remove all build files')
    parsed_args = parser.parse_args(args)

    clean(parsed_args.all)

def clean(all: bool=False):
    build_dir = config.get_build_dir()
    if all:
        if os.path.isdir(build_dir):
            print('Removing all build files')
            shutil.rmtree(build_dir)
        else:
            print('Nothing to clean')
    else:
        if os.path.isdir(os.path.join(build_dir, 'bin')):
            print('Removing DrkCraft build files')
            shutil.rmtree(os.path.join(build_dir, 'bin'))
            shutil.rmtree(os.path.join(build_dir, 'DrkCraft.dir'))
        else:
            print('Nothing to clean')

if __name__ == '__main__':
    sys.exit(main(sys.argv))
