from typing import *
import argparse
import os.path
import enum
import platform

class Option(enum.Enum):
    build_config = 0
    en_profiling = 1
    en_dev_mode  = 2
    en_trace_log = 3

class BuildConfig(enum.Enum):
    Unknown        = 0
    Debug          = 1
    Release        = 2
    RelWithDebInfo = 3
    Dist           = 4

    @classmethod
    def from_str(cls, s: str):
        match s.lower():
            case 'debug'          : return BuildConfig.Debug
            case 'release'        : return BuildConfig.Release
            case 'relwithdebinfo' : return BuildConfig.RelWithDebInfo
            case 'dist'           : return BuildConfig.Dist
            case _: return cls.Unknown

    def __str__(self):
        match self:
            case BuildConfig.Debug          : return 'Debug'
            case BuildConfig.Release        : return 'Release'
            case BuildConfig.RelWithDebInfo : return 'RelWithDebInfo'
            case BuildConfig.Dist           : return 'Dist'
            case _: return ''

def parse_args(argv: Sequence[str], options: Sequence[Option], description='') -> argparse.Namespace:
    options = rm_duplicates_from_list(options)

    parser = argparse.ArgumentParser(description=description)
    for option in options:
        match option:
            case Option.build_config : parser.add_argument(
                'build_config', metavar='config',
                type=BuildConfig.from_str, nargs='?', default=BuildConfig.Unknown, choices=( e.value for e in BuildConfig ),
                help='[Debug|Release|RelWithDebInfo|Dist] Dist overwrites all other options'
            )
            case Option.en_profiling : parser.add_argument(
                '--profile', dest='en_profiling', action='store_true', help='Enable profiling'
            )
            case Option.en_dev_mode : parser.add_argument(
                '--dev', dest='en_dev_mode', action='store_true', help='Enable Dev mode'
            )
            case Option.en_trace_log : parser.add_argument(
                '--trace', dest='en_trace_log', action='store_true', help='Enable console trace logging'
            )
    args = parser.parse_args(argv)

    if Option.build_config in options:
        if args.build_config == BuildConfig.Unknown:
            args.build_config = BuildConfig.Debug
            en_profiling = True
            en_dev_mode  = True
            en_trace_log = False
        elif args.build_config == BuildConfig.Dist:
            en_profiling = False
            en_dev_mode  = False
            en_trace_log = False

        if Option.en_profiling in options: args.en_profiling = en_profiling
        if Option.en_dev_mode  in options: args.en_dev_mode  = en_dev_mode
        if Option.en_trace_log in options: args.en_trace_log = en_trace_log

    return args

def get_build_dir() -> str:
    return 'build'

def get_platform() -> str:
    return platform.system()

def get_exe(build_config: BuildConfig):
    if build_config == BuildConfig.Dist:
        config_str = str(BuildConfig.Release)
    else:
        config_str = str(build_config)

    if get_platform() == 'Windows':
        extension = '.exe'
    else:
        extension = ''

    return os.path.join(get_build_dir(), 'bin', config_str, 'DrkCraft' + extension)

def get_version() -> str:
    with open('VERSION.txt', 'r') as f:
        return f.read().strip()

def rm_duplicates_from_list(lst: Sequence[Any]):
    return [ item for i, item in enumerate(lst) if item not in lst[:i] ]
