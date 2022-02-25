import argparse

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('filename', type=str)
    parser.add_argument('DrkCraft_license', type=str)
    parser.add_argument('lib_license', type=str)
    args = parser.parse_args()

    with open(args.DrkCraft_license, 'r') as f:
        drkcraft_license = f.read()

    with open(args.lib_license, 'r') as f:
        lib_license = f.read()

    with open(args.filename, 'w') as f:
        f.write('========== DrkCraft ===========================================================\n')
        f.write('\n')
        f.write(drkcraft_license)
        f.write('\n')
        f.write('===============================================================================\n')
        f.write('\n')
        f.write(lib_license)
