import argparse

def on_off_to_bool(string: str):
    if string == 'ON': return True
    if string == 'OFF': return False
    return None

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('filename')
    parser.add_argument('version')
    parser.add_argument('platform')
    parser.add_argument('build_config')
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
        f.write('Profiling: {}\n'.format('Enabled' if args.en_profile else 'Disabled'))
        f.write('Dev mode: {}\n'   .format('Enabled' if args.en_dev_mode else 'Disabled'))
        f.write('Trace logging: {}\n'.format('Enabled' if args.en_trace_log else 'Disabled'))
