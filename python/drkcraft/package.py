import os.path

import drkcraft

class PackageError(Exception):
    pass

class Package:
    def __init__(self, package_name: str='DrkCraft'):
        validate_package(package_name)

        self.version = find_package_version(package_name)

def validate_package(package_name: str):
    if os.path.isdir(package_name):
        exe = os.path.join(package_name, drkcraft.get_exe_name())
        if os.path.isfile(exe):
            if os.access(exe, os.X_OK):
                return

    raise PackageError(f'{package_name} is not a DrkCraft package')

def find_package_version(package_name) -> str:
    return drkcraft.read_version_file(os.path.join(package_name, 'about', 'VERSION.txt'))
