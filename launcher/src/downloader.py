from typing import *
from pathlib import Path
from urllib.parse import urlparse, urljoin
import shutil

import requests
from tqdm import tqdm

from config import *

REPO_URL = 'http://derek-henderson.dev/software/DrkCraft'

class Downloader:
    def __init__(self, temp: Path=TEMP_DIR, cache: Path=CACHE_DIR):
        self.temp_dir = Path(temp)
        self.cache_dir = Path(cache)

    def download(self, filename: str, dest: Optional[Path]=None) -> bool:
        url = urljoin(REPO_URL, filename)

        if dest is None:
            dest = self.cache_dir / filename

        temp_file = self.temp_dir / filename

        response = requests.get(url, stream=True)

        if response.ok:
            with open(temp_file, 'wb') as f:
                for data in tqdm(response.iter_content()):
                    f.write(data)

            shutil.move(temp_file, dest)
            return True
        else:
            return False
