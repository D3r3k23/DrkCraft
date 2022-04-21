
import os.path
import subprocess

def gen_glad():
    subprocess.run([ 'python', '-m', 'glad',
        '--generator', 'c', '--no-loader',
        '--out-path', os.path.join('lib', 'glad')
    ])
