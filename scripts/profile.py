from typing import *
import argparse
import json
import dataclasses
import os.path

from pprint import pprint

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

def main():
    parser = argparse.ArgumentParser(description='Analyzes profiler results')
    parser.add_argument('profile', nargs='?', type=str, default=os.path.join('data', 'profile', 'results.json'))
    args = parser.parse_args()

    print(f'Loading "{args.profile}"')
    profile_json = read_json(args.profile)

    if profile_json is None:
        print('Error: Could not open profile results')
    else:
        analyze_profile(profile_json)

def analyze_profile(profile_json: Mapping):
    profile = profile_json['traceEvents'][1:]

    functionstats: Dict[str, float] = {}
    scopestats:   Dict[str, float] = {}
    threadstats: Dict[str, float] = {}

    def load_results():
        nonlocal functionstats, scopestats, threadstats

        functions: List[DurationEvent] = []
        scopes:   List[DurationEvent] = []
        threads: List[DurationEvent] = []
        events: List[InstantEvent]  = []

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

        functionstats = add_durations(functions)
        scopestats   = add_durations(scopes)
        threadstats = add_durations(threads)

    def print_stats(stats: Dict[str, float]):
        for i, name in enumerate(sorted(stats.keys(), key=stats.get, reverse=True)[:10], 1):
            duration = stats[name]
            print(f'{i}. {name}: {duration / 1000:.2f}ms')

    load_results()

    print('Profiled functions:')
    print_stats(functionstats)
    print()
    print('Profiled scopes:')
    print_stats(scopestats)
    print()
    print('Profiled threads:')
    print_stats(threadstats)

def add_durations(events: List[DurationEvent]) -> Dict[str, float]:
    result: Dict[str, float] = {}
    for event in events:
        if not event.name in result:
            result[event.name] = event.duration
        else:
            result[event.name] += event.duration
    return { k: result[k] for k in sorted(result.keys(), key=lambda kk: result[kk], reverse=True) }

def read_json(filename: str) -> Optional[Mapping]:
    try:
        with open(filename, 'r') as f:
            return json.load(f)
    except (OSError, IOError):
        return None

if __name__ == '__main__':
    main()
