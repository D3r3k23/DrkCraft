from typing import *
import subprocess
import sys
import os.path
from pathlib import Path

def main() -> Optional[int]:
    exe = os.path.join('dist', 'Drkcraft Launcher' + '.exe' if True else '')

    try:
        subprocess.run([ exe ])
    except subprocess.CalledProcessError:
        return 1

if __name__ == '__main__':
    sys.exit(main())
