import argparse

title = (
    R'$$$$$$$\            $$\        $$$$$$\                       $$$$$$\    $$ ''\n'
    R'$$  __$$\           $$ |      $$  __$$\                     $$  __$$\   $$ |''\n'
    R'$$ |  $$ | $$$$$$\  $$ |  $$\ $$ /  \__| $$$$$$\   $$$$$$\  $$ /  \__|$$$$$$ ''\n'
    R'$$ |  $$ |$$  __$$\ $$ | $$  |$$ |      $$  __$$\  \____$$\ $$$$\     \_$$  _|''\n'
    R'$$ |  $$ |$$ |  \__|$$$$$$  / $$ |      $$ |  \__| $$$$$$$ |$$  _|      $$ |   ''\n'
    R'$$ |  $$ |$$ |      $$  _$$<  $$ |  $$\ $$ |      $$  __$$ |$$ |        $$ |$$\ ''\n'
    R'$$$$$$$  |$$ |      $$ | \$$\ \$$$$$$  |$$ |      \$$$$$$$ |$$ |        \$$$$  | ''\n'
    R'\_______/ \__|      \__|  \__| \______/ \__|       \_______|\__|         \____/   ''\n'
)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('filename', type=str)
    parser.add_argument('version',  type=str)
    parser.add_argument('platform', type=str, choices=['windows', 'linux'])
    args = parser.parse_args()

    exe: str = 'DrkCraft' + '.exe' if args.platform == 'windows' else ''

    with open(args.filename, 'w') as f:
        f.write(title)
        f.write('\n')
        f.write(f'To play, run the file {exe} in the root DrkCraft directory\n')
        f.write(f'Command line usage: DrkCraft/$: ./{exe} [--dev]\n')
        f.write('\n')
        f.write('Options:\n')
        f.write('  --dev   Enable Dev mode (Only available in some builds)\n')
        f.write('\n')
        f.write('For help, see the following: ')
        f.write(f'https://github.com/D3r3k23/DrkCraft/tree/v{args.version}/doc/manual.md\n')
        f.write(' - Or .md files in DrkCraft/doc\n')
        f.write('\n')
        f.write('License information available in DrkCraft/about/LICENSE.txt and in DrkCraft/assets\n')
