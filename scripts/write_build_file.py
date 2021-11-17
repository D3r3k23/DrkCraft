import argparse

def on_off_to_bool(str):
    if str == 'ON':  return True
    if str == 'OFF': return False
    return None

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('fn')
    parser.add_argument('version')
    parser.add_argument('platform')
    parser.add_argument('build_config')
    parser.add_argument('en_profile',  type=on_off_to_bool)
    parser.add_argument('en_dev_mode', type=on_off_to_bool)
    parser.add_argument('dist',        type=on_off_to_bool)
    args = parser.parse_args()

    with open(args.fn, 'w') as f:
        f.write('Version: {}  \n'.format(args.version))
        f.write('Platform: {} \n'.format(args.platform))
        f.write('Config: {}   \n'.format(args.build_config))
        f.write('Profiling: {}\n'.format('Enabled' if args.en_profile  else 'Disabled'))
        f.write('Dev Mode: {} \n'.format('Enabled' if args.en_dev_mode else 'Disabled'))
        f.write('Dist: {}     \n'.format('True'    if args.dist        else 'False'))