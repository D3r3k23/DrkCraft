from typing import *
import argparse
import shutil
import os.path
import sys

def main(argv: list[str]=sys.argv) -> Optional[int]:
    if sys.argv[0] == 'drkcraft':
        sys.argv[0] += ' clean'

    parser = argparse.ArgumentParser(description='Removes build files')
    parser.add_argument('--all', action='store_true')
    args = parser.parse_args(argv)

    clean(args.all)

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
