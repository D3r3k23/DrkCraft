from typing import *
import argparse
import sys
import os
import re
from datetime import datetime
from pathlib import Path

def main(argv: list[str]=sys.argv) -> Optional[int]:
    prog, args = argv[0], argv[1:]

    description = 'Cleans log directory'
    usage = f'{prog} [log_dir] [--max_age MAX_AGE]'

    parser = argparse.ArgumentParser(prog=prog, description=description, usage=usage)
    parser.add_argument('log_dir', type=str, nargs='?', default=os.path.join('data', 'logs'), help='Log directory')
    parser.add_argument('--max_age', type=int, default=5, help='Max age (days) for log files to keep')
    parsed_args = parser.parse_args(args)

    if not os.path.isdir(parsed_args.log_dir):
        print(f'Error: log directory "{parsed_args.log_dir}" not found')
        return 1

    clean_logs(parsed_args.log_dir, parsed_args.max_age)

def clean_logs(logdir: str, max_age: int=5):
    now = datetime.now()
    logs = [ log for log in os.listdir(logdir) if Path(log).suffix == '.log' ]
    for log in logs:
        date = get_date_from_log_name(log)
        if date is not None and (now - date).days > max_age:
            print(f'Deleting log: {log}')
            os.remove(os.path.join(logdir, log))

def get_date_from_log_name(log: str) -> Optional[datetime]:
    pattern = re.compile('_\d\d\d\d-\d\d-\d\d_')
    result  = pattern.search(log)
    if result:
        date_string    = result[0].strip('_')
        date_components = date_string.split('-')
        year, month, day = date_components
        date = datetime(int(year), int(month), int(day))
        return date
    else:
        return None

if __name__ == '__main__':
    sys.exit(main(sys.argv))
