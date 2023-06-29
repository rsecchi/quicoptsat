import subprocess
import datetime
import matplotlib.pyplot as plt
import os

def get_connection_time(pcap_path):
    out = subprocess.run(f'tcpdump -n -r {pcap_path} -tt', stdout=subprocess.PIPE, shell=True)
    lines = out.stdout.decode().splitlines()
    start = None
    end = None
    for line in lines:
        if '10.0.0.1.80' in line:
            if not start:
                start = line.split()[0]
            else:
                end = line.split()[0]

    duration_dt = (datetime.datetime.fromtimestamp(float(end)) - datetime.datetime.fromtimestamp(float(start)))
    return duration_dt.total_seconds()


def plot_completion_time(root, paths):
    fsize_time = {}
    for path in paths:
        server_pcap_path = os.path.join(root, f'{path}', 'server.pcap')
        completion_time = get_connection_time(server_pcap_path)
        fsize_time[path] = completion_time

    file_sizes, completion_times = zip(*fsize_time.items())
    plt.scatter(file_sizes, completion_times)
    plt.ylabel('Time (s)')
    plt.xlabel('File Size (bytes)')
    plt.savefig('plt.png')


if __name__ == '__main__':
    f_sizes = list(range(50_000, 5_000_001, 50_000))
    plot_completion_time('/vagrant/logs/DSL/cubic_cr', f_sizes)

