from typing import Optional
from datetime import datetime
import argparse
import os
import re

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('log_dir', type=str, nargs='?', default=os.path.join('data', 'logs'))
    parser.add_argument('--max_age', type=int, default=3)
    args = parser.parse_args()

    clean_logs(args.log_dir, args.max_age)

def clean_logs(logdir: str, maxage: int):
    now = datetime.now()
    logs = os.listdir(logdir)
    for log in logs:
        date = get_date_from_log_name(log)
        if date is not None and (now - date).days > maxage:
            print(f'Deleting log: {log}')
            os.remove(os.path.join(logdir, log))

def get_date_from_log_name(log: str) -> Optional[datetime]:
    pattern = re.compile('_\d\d\d\d-\d\d-\d\d_')
    result  = pattern.search(log)
    if result:
        datestring   = result[0].strip('_')
        datecomponents = datestring.split('-')
        year, month, day = datecomponents
        date = datetime(int(year), int(month), int(day))
        return date
    else:
        return None

if __name__ == '__main__':
    main()
