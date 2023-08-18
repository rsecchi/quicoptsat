import os
import json
import sys
import argparse

from get_connection_time import get_connection_time

import numpy as np

def _analyze():
    to_analyze = '/vagrant/logs/pacing/LEO/cubic_cr/IW'
    file_sizes = range(50_000, 5_000_001, 50_000)
    repeats = range(2, 6)

    completion_times = {}

    for file_size in file_sizes:
        for repeat in repeats:
            pcap_path = os.path.join(to_analyze, f'{file_size}', f'{repeat}', 'server.pcap')
            comp_time_fs = completion_times.get(file_size, [])
            comp_time_fs.append(get_connection_time(pcap_path))
            completion_times[file_size] = comp_time_fs
    
    # print(completion_times)
    for fs, comp_times in completion_times.items():
        print(fs, comp_times, f'Standard deviation {np.std(comp_times):.3f}')


def parse_completion_times(root, subdirs, experiment_paths):
    
    completion_time_all = {}
    for subdir in subdirs:
        completion_times_alg = {}
        
        for experiment_path in experiment_paths:
            #TODO: Take median value from the following
            pcap_path = os.path.join(root, subdir, f'{experiment_path}', '1', 'server.pcap')
            completion_time = get_connection_time(pcap_path)
            completion_times_alg[experiment_path] = completion_time

        completion_time_all[subdir] = completion_times_alg

    PREFIX = '/vagrant/logs/'
    if PREFIX not in root:
        print('Check the log root argument')
    else:
        suffix=root.split(PREFIX)[1]
        parsed_save_path = os.path.join(PREFIX, 'parsed', suffix)
        os.makedirs(parsed_save_path, exist_ok=True)
        with open(os.path.join(parsed_save_path, 'completion_times.json'), 'w') as f:
            json.dump(completion_time_all, f)


if __name__ == '__main__':
    if len(sys.argv) == 1:
        _analyze()
        exit()
        experiment_paths = list(range(50_000, 5_000_001, 50_000))
        parse_completion_times(root='/vagrant/logs/pacing/LEO/cubic_cr', subdirs=['IW', '12BDP', '23BDP'], experiment_paths=experiment_paths)
    else:
        parser = argparse.ArgumentParser()
        parser.add_argument('--root', help='Root directory that will combine subdirs. Nomally logs/<LINK>/<ALG>', required=True)
        parser.add_argument('--subdirs', nargs='+', help='first level subdirectories to parse from root', required=True)
        parser.add_argument('--experiment_paths', nargs='+', help='Final level subdirectories to parse from root. These contain the pcap files required for parsing', required=True)

        args = parser.parse_args()
        parse_completion_times(root=args.root, subdirs=args.subdirs, experiment_paths=args.experiment_paths)
