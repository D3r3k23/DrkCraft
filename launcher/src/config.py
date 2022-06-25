from pathlib import Path
import platform

from semver import VersionInfo as Version

from _version import __version__
VERSION = Version.parse(__version__)

PLATFORM = platform.system()

LOG_DIR = Path('Launcher/logs')
RESOURCE_DIR = Path('Launcher/resources')

TEMP_DIR = Path('.temp')
CACHE_DIR = Path('.cache')
