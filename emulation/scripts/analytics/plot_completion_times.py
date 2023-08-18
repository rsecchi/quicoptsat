import matplotlib.pyplot as plt
import json
import os
import sys
import argparse


def plot_completion_time(parsed_path, figname='plt.png'):
    parsed_completion_times = {}
    with open(parsed_path) as f:
        parsed_completion_times = json.load(f)

    link_info = None
    parsed_rel_root = parsed_path.split('parsed/')[1]
    if parsed_rel_root.startswith('pacing/'):
        parsed_rel_root = parsed_rel_root.split('pacing/')[1]

    link_name = parsed_rel_root.split('/')[0] 
    link_models_path = '/vagrant/network_models/links'
    if link_name in ['LEO', 'GEO']:
        link_info_file = os.path.join(link_models_path, f'{link_name}.json')
        with open(link_info_file) as f:
            link_info = json.load(f)['changes'][0]
    else:
        print(f"Unrecognized Link type {link_name}")
        return

    RTT = int(link_info['rtt'])
    RTT_S = RTT / 1000

    for label, alg_parsed_completion_times in parsed_completion_times.items():

        file_sizes, completion_times = zip(*alg_parsed_completion_times.items())
        completion_times_rtt = [x / RTT_S for x in completion_times]
        # print(completion_times, completion_times_rtt)
        file_sizes = [int(x) for x in file_sizes]
        # completion_times = [float(x) for x in completion_times]
        plt.plot(file_sizes, completion_times_rtt, label=label)


    plt.ylabel('# of RTTs')
    plt.xlabel('File Size (bytes)')
    y_max = 25 if 'GEO' in figname else 45
    plt.ylim(bottom=0, top=y_max)
    plt.xlim(left=0)
    plt.legend()
    plt.savefig(figname, bbox_inches='tight')



if __name__ == '__main__':
    if len(sys.argv) == 1:
        plot_completion_time(parsed_path='/vagrant/logs/parsed/pacing/LEO/cubic_cr/completion_times.json')
    else:
        parser = argparse.ArgumentParser()
        parser.add_argument('--figname', help='Name of the figure to save (including path)', required=True)
        parser.add_argument('--plot_data_file', help='File containing data to be plotted', required=True)

        args = parser.parse_args()

        plot_completion_time(parsed_path=args.plot_data_file, figname=args.figname)
