import os.path
import platform

from ._version import __version__
VERSION = __version__

PLATFORM = platform.system()

LOG_DIR = os.path.join('Launcher', 'logs')
RESOURCE_DIR = os.path.join('Launcher', 'resources')
