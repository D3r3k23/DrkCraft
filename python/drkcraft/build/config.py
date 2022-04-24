from typing import *
import argparse
import os.path
import enum

import drkcraft

class Option(enum.Enum):
    build_config = 0
    en_profiling = 1
    en_dev_mode  = 2
    en_trace_log = 3
    skip_build   = 4

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

    def to_cmake_build_type(self) -> str:
        match self:
            case BuildConfig.Debug          : return 'Debug'
            case BuildConfig.Release        : return 'Release'
            case BuildConfig.RelWithDebInfo : return 'RelWithDebInfo'
            case BuildConfig.Dist           : return 'Release'
            case _: return 'Debug'

def parse_args(argv: Sequence[str], options: Sequence[Option], description: Optional[str]=None) -> argparse.Namespace:
    prog = argv[0]
    args = argv[1:]
    options = rm_duplicates_from_list(options)

    usage = f'{prog} [-h]'
    if Option.build_config in options: usage += ' [config]'
    if Option.en_profiling in options: usage += ' [--profile]'
    if Option.en_dev_mode  in options: usage += ' [--dev]'
    if Option.en_trace_log in options: usage += ' [--trace]'
    if Option.skip_build   in options: usage += ' [--skip-build]'

    cfg_choices = [ c for c in BuildConfig ]
    cfg_help = drkcraft.make_options_str([ str(c) for c in cfg_choices if c != BuildConfig.Unknown ])
    cfg_help += ' Dist overrides all other options'

    parser = argparse.ArgumentParser(prog=prog, description=description, usage=usage)
    if Option.build_config in options: parser.add_argument(
        'build_config', metavar='config', type=BuildConfig.from_str, nargs='?', default='', choices=cfg_choices, help=cfg_help
    )
    if Option.en_profiling in options: parser.add_argument(
        '--profile', dest='en_profiling', action='store_true', help='Enable profiling'
    )
    if Option.en_dev_mode in options: parser.add_argument(
        '--dev', dest='en_dev_mode', action='store_true', help='Enable Dev mode'
    )
    if Option.en_trace_log in options: parser.add_argument(
        '--trace', dest='en_trace_log', action='store_true', help='Enable console trace logging'
    )
    if Option.skip_build in options: parser.add_argument(
        '--skip-build', dest='skip_build', action='store_true', help='Skips DrkCraft build step'
    )
    parsed_args = parser.parse_args(args)

    if Option.build_config in options:
        if parsed_args.build_config == BuildConfig.Unknown:
            parsed_args.build_config = BuildConfig.Debug
            if Option.en_profiling in options: parsed_args.en_profiling = True
            if Option.en_dev_mode  in options: parsed_args.en_dev_mode  = True
            if Option.en_trace_log in options: parsed_args.en_trace_log = False
        elif parsed_args.build_config == BuildConfig.Dist:
            if Option.en_profiling in options: parsed_args.en_profiling = False
            if Option.en_dev_mode  in options: parsed_args.en_dev_mode  = False
            if Option.en_trace_log in options: parsed_args.en_trace_log = False

    return parsed_args

def get_build_dir() -> str:
    return 'build'

def get_exe(build_config: BuildConfig):
    if build_config == BuildConfig.Dist:
        config_str = str(BuildConfig.Release)
    else:
        config_str = str(build_config)

    return os.path.join(get_build_dir(), 'bin', config_str, drkcraft.get_exe_name())

def get_version() -> str:
    return drkcraft.read_version_file('VERSION.txt')

def rm_duplicates_from_list(lst: Sequence[Any]):
    return [ item for i, item in enumerate(lst) if item not in lst[:i] ]
