from typing import *
from contextlib import contextmanager
import os
import time
import errno
import platform

def read_version_file(filename: str='VERSION.txt') -> str:
    try:
        with open(filename, 'r') as f:
            return f.read().strip()
    except (OSError, IOError):
        raise FileNotFoundError(errno.ENOENT, os.strerror(errno.ENOENT), filename)

def get_platform() -> str:
    return platform.system()

def get_exe_name() -> str:
    return 'DrkCraft' + '.exe' if get_platform() == 'Windows' else ''

class Timer:
    def __init__(self):
        self.start = self.now()

    def elapsed(self) -> float:
        return self.now() - self.start

    def __str__(self) -> str:
        elapsed = self.elapsed()
        elapsed_min = round(elapsed) // 60
        elapsed_sec = round(elapsed % 60)
        return f'{elapsed_min}min {elapsed_sec}s'

    @staticmethod
    def now() -> float:
        return time.perf_counter()

def get_file_size(filename: str) -> Optional[int]:
    try:
        return os.path.getsize(filename)
    except FileNotFoundError:
        return None

def get_file_size_mb(filename: str) -> Optional[float]:
    bytes = get_file_size(filename)
    return bytes / 10**6 if bytes else None

class DirNotFoundError(Exception):
    def __init__(self, dir: str):
        super().__init__(f'Directory "{dir}" does not exist')

@contextmanager
def cd(dir: str):
    prev = os.getcwd()

    try:
        os.chdir(os.path.expanduser(dir))
    except FileNotFoundError:
        raise DirNotFoundError(dir)

    try:
        yield
    finally:
        os.chdir(prev)

def make_options_str(lst: list[str]) -> str:
    return f'[{"".join(f"{c}|" for c in lst[:-1])}{lst[-1]}]'
