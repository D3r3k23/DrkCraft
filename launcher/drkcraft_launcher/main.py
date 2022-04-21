from typing import *
import argparse
import sys

from _version import __version__
import launcher

def main(argv: list[str]) -> Optional[int]:
    parser = argparse.ArgumentParser().add_argument('-V', '--version', action='version', version=__version__)
    parser.parse_args()

    launcher.run()

if __name__ == '__main__':
    sys.exit(main(sys.argv))
