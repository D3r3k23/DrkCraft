from typing import *
import argparse

def on_off_to_bool(string: str) -> Optional[bool]:
    if string.upper() == 'ON': return True
    if string.upper() == 'OFF': return False
    return None

def bool_to_en_dis(enabled: bool) -> str:
    return 'Enabled' if enabled else 'Disabled'

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('filename',  type=str)
    parser.add_argument('version',    type=str)
    parser.add_argument('platform',    type=str)
    parser.add_argument('build_config', type=str)
    parser.add_argument('en_profile',   type=on_off_to_bool)
    parser.add_argument('en_dev_mode',  type=on_off_to_bool)
    parser.add_argument('en_trace_log', type=on_off_to_bool)
    parser.add_argument('dist',         type=on_off_to_bool)
    args = parser.parse_args()

    with open(args.filename, 'w') as f:
        f.write('Version: {}\n'  .format(args.version))
        f.write('Platform: {}\n'.format(args.platform))
        f.write('Config: {}\n' .format(args.build_config))
        f.write('Dist: {}\n'  .format(args.dist))
        f.write('Profiling: {}\n'.format(bool_to_en_dis(args.en_profile)))
        f.write('Dev mode: {}\n'   .format(bool_to_en_dis(args.en_dev_mode)))
        f.write('Trace logging: {}\n'.format(bool_to_en_dis(args.en_trace_log)))
