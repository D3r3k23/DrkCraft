from typing import *
import subprocess
import os.path
import sys

def main() -> Optional[int]:
    try:
        subprocess.run([ sys.executable, '-m', 'glad',
            '--generator' 'c',
            '--no-loader',
            ' --out-path', os.path.join('lib', 'glad')
        ])
    except subprocess.SubprocessError:
        return 1

if __name__ == '__main__':
    sys.exit(main())
