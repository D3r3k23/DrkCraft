from typing import *
from pathlib import Path
from urllib.parse import urljoin
import shutil

import semver

from config import *
from downloader import Downloader

class LauncherInstaller:
    def __init__(self, drkcraft_dir: Path):
        self.drkcraft_dir = drkcraft_dir
        self.downloader = LauncherDownloader(TEMP_DIR)
        self.downloaded = False

    def download(self):
        self.downloader.download()
        self.downloaded = True

    def install(self):
        if not self.downloaded:
            self.download()

        launcher_zip = self.downloader.dest / 'drkcraft-launcher.zip'
        launcher_package = self.downloader.dest / 'Launcher'

        # unzip to launcher_package
        # shutil.copy(launcher_package, self.drkcraft_dir)

        launcher_zip.unlink()
        shutil.rmtree(launcher_package)
        self.downloaded = False

        launcher_link = self.drkcraft_dir / 'DrkCraft Launcher'
        if launcher_link.exists():
            launcher_link.unlink()

        # create link: self.drkcraft_dir -> self.drkcraft_dir / 'Launcher' / 'drkcraft-launcher.exe'

    def update(self):
        if not self.downloaded and self.downloader.check_update_available():
            self.download()
            self.install()

    def create_windows_shortcut(self):
        ...

class LauncherDownloader:
    def __init__(self, download_dir: Path=TEMP_DIR):
        self.dest = download_dir
        self.downloader = Downloader()
        self.check_latest_launcher_version()

    def check_latest_launcher_version(self):
        self.latest_version = Version.parse('0.0.0')

    def update_available(self) -> bool:
        return self.latest_version > VERSION

    def check_update_available(self) -> bool:
        self.check_latest_launcher_version()
        return self.update_available()

    def download(self, version: Optional[Version]=None):
        if version is None:
            version = self.latest_version

        self.downloader.download(f'launcher/v{version}/drkcraft-launcher.zip')
