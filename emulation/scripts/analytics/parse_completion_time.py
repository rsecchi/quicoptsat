import os
import json

from get_connection_time import get_connection_time

def parse_completion_times(root, subdirs, experiment_paths):
    
    completion_time_all = {}
    for subdir in subdirs:
        completion_times_alg = {}
        
        for experiment_path in experiment_paths:
            pcap_path = os.path.join(root, subdir, f'{experiment_path}', 'server.pcap')
            completion_time = get_connection_time(pcap_path)
            completion_times_alg[experiment_path] = completion_time

        completion_time_all[subdir] = completion_times_alg

    PREFIX = '/vagrant/logs/'
    if PREFIX not in root:
        print('Check the log root argument')
    else:
        suffix=root.split(PREFIX)[1]
        parsed_save_path = os.path.join(PREFIX, 'parsed', suffix)
        os.makedirs(parsed_save_path)
        with open(os.path.join(parsed_save_path, 'completion_times.json'), 'w') as f:
            json.dump(completion_time_all, f)


if __name__ == '__main__':
    experiment_paths = list(range(50_000, 5_000_001, 50_000))
    parse_completion_times(root='/vagrant/logs/LEO/cubic_cr', subdirs=['IW', '12BDP', '23BDP'], experiment_paths=experiment_paths)

