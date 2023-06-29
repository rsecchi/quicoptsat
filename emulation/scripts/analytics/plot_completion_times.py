import matplotlib.pyplot as plt
import json


def plot_completion_time(parsed_path):
    parsed_completion_times = {}
    with open(parsed_path) as f:
        parsed_completion_times = json.load(f)


    for label, alg_parsed_completion_times in parsed_completion_times.items():

        file_sizes, completion_times = zip(*alg_parsed_completion_times.items())
        completion_times_rtt = [x / 0.05 for x in completion_times]
        # print(completion_times, completion_times_rtt)
        file_sizes = [int(x) for x in file_sizes]
        # completion_times = [float(x) for x in completion_times]
        plt.plot(file_sizes, completion_times_rtt, label=label)


    plt.ylabel('# of RTTs')
    plt.xlabel('File Size (bytes)')
    plt.ylim(bottom=0)
    plt.xlim(left=0)
    plt.legend()
    plt.savefig('completion_times_RTT.png', bbox_inches='tight')



if __name__ == '__main__':
    plot_completion_time(parsed_path='/vagrant/logs/parsed/LEO/cubic_cr/completion_times.json')