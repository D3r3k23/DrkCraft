from typing import *
import argparse
import shutil
import os.path
import sys

def main(argv: list[str]=sys.argv) -> Optional[int]:
    prog = argv[0]
    args = argv[1:]

    description = 'Removes DrkCraft build files'
    usage = f'{prog} [-h] [--all]'

    parser = argparse.ArgumentParser(prog=prog, description=description, usage=usage)
    parser.add_argument('--all', action='store_true', help='Remove all build files')
    parsed_args = parser.parse_args(args)

    clean(parsed_args.all)

def clean(all: bool=False):
    if all:
        if os.path.isdir('build'):
            print('Removing all build files')
            shutil.rmtree('build')
        else:
            print('Nothing to clean')
    else:
        if os.path.isdir('build/bin'):
            print('Removing DrkCraft build files')
            shutil.rmtree('build/bin')
        else:
            print('Nothing to clean')

if __name__ == '__main__':
    sys.exit(main(sys.argv))
