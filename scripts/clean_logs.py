import os
import re
from datetime import datetime

LOG_DIR = 'data/logs'
MAX_AGE = 3 # days

def get_date_from_log_name(log):
    pattern = re.compile('_\d\d\d\d-\d\d-\d\d_')
    result = pattern.search(log)
    if result:
        datestring = result[0].strip('_')
        datecomponents = datestring.split('-')
        year, month, day = datecomponents
        date = datetime(int(year), int(month), int(day))
        return date
    else:
        return None

if __name__ == '__main__':
    now = datetime.now()
    logs = os.listdir(LOG_DIR)
    for log in logs:
        date = get_date_from_log_name(log)
        if date and (now - date).days > MAX_AGE:
            print(f'Deleting log: {log}')
            os.remove(os.path.join(LOG_DIR, log))
