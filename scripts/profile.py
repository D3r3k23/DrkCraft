from typing import *
import argparse
import sys
import os.path
import dataclasses
import json

@dataclasses.dataclass
class DurationEvent:
    name: str
    timestep: float
    duration: float

    @staticmethod
    def from_json(jobj: Mapping):
        return DurationEvent(jobj['name'], jobj['ts'], jobj['dur'])

@dataclasses.dataclass
class InstantEvent:
    name: str
    timestep: float

    @staticmethod
    def from_json(jobj: Mapping):
        return InstantEvent(jobj['name'], jobj['ts'])

def main() -> Optional[int]:
    parser = argparse.ArgumentParser(description='Analyzes profiler results')
    parser.add_argument('profile', nargs='?', type=str, default=os.path.join('data', 'profile', 'results.json'))
    args = parser.parse_args()

    if not os.path.isfile(args.profile):
        print(f'Error: profile file "{args.profile}" not found')
        return 1

    print(f'Loading "{args.profile}"')
    try:
        profile_json = load_json(args.profile)
    except json.JSONDecodeError as e:
        print(f'JSON Error: {e.msg}')
        return 1

    analyze_profile(profile_json)

def analyze_profile(profile_json: Mapping):
    profile = profile_json['traceEvents'][1:]

    functions: list[DurationEvent] = []
    scopes:   list[DurationEvent] = []
    threads: list[DurationEvent] = []
    events: list[InstantEvent]  = []

    for trace in filter(lambda t: 'ph' in t and 'cat' in t, profile):
        match trace['ph']:
            case 'X':
                match trace['cat']:
                    case 'function' : functions.append(DurationEvent.from_json(trace))
                    case 'scope'    : scopes.append(DurationEvent.from_json(trace))
                    case 'thread'   : threads.append(DurationEvent.from_json(trace))
            case 'i':
                match trace['cat']:
                    case 'event' : events.append(InstantEvent.from_json(trace))
            case 's':
                pass
            case 'f':
                pass

    functionstats: dict[str, float] = add_durations(functions)
    scopestats:   dict[str, float] = add_durations(scopes)
    threadstats: dict[str, float] = add_durations(threads)

    print('Profiled functions:')
    print_stats(functionstats)
    print()
    print('Profiled scopes:')
    print_stats(scopestats)
    print()
    print('Profiled threads:')
    print_stats(threadstats)

    # Averate time per call

    # Events - frame time

def add_durations(events: list[DurationEvent]) -> dict[str, float]:
    result: dict[str, float] = {}
    for event in events:
        if event.name not in result:
            result[event.name] = event.duration
        else:
            result[event.name] += event.duration
    return { k: result[k] for k in sorted(result.keys(), key=lambda name: result[name], reverse=True) }

def print_stats(stats: dict[str, float]):
    for i, name in enumerate(sorted(stats.keys(), key=stats.get, reverse=True)[:10], 1):
        duration = stats[name]
        print(f'{i}. {name}: {duration / 1000:.2f}ms')

def load_json(filename: str) -> Optional[Mapping]:
    try:
        with open(filename, 'r') as f:
            return json.load(f)
    except (OSError, IOError):
        return None

if __name__ == '__main__':
    sys.exit(main())
