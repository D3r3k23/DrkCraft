from pathlib import Path
import platform

from ._version import __version__
VERSION = __version__

PLATFORM = platform.system()

LOG_DIR = Path('Launcher/logs')
RESOURCE_DIR = Path('Launcher/resources')
