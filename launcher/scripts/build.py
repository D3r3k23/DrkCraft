from typing import *
import sys
import os.path

import PyInstaller.__main__

def main() -> Optional[int]:
    return PyInstaller.__main__.run([
        ''
    ])

if __name__ == '__main__':
    sys.exit(main())
