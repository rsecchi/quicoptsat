import matplotlib.pyplot as plt
import argparse
import sys
import os

USE_RELATIVE_TIME = True

def get_cwnd_info(kern_log_path):
    data = []
    with open(kern_log_path) as f:
        for line in f:
            # Process ack event
            if 'ACK Received' in line and 'send window:' in line:
                event_data = {}
                tokens = line.split()
                if USE_RELATIVE_TIME:
                    time = tokens[5][1:-1]
                    event_data['time'] = time
                cwnd = line.split('send window: ')[1].split()[0]
                event_data['cwnd'] = cwnd
                # ssthresh = line.split('ssthresh: ')[1].split()[0]
                # event_data['ssthresh'] = ssthresh
                data.append(event_data)
    
    return data


def plot_cwnd(data, savepath, file_type):
    # data is [{'time': str, 'cwnd': str, 'ssthresh': cwnd}]
    if USE_RELATIVE_TIME:
        first_ack = data[0]['time']
        first_ack = float(first_ack)
        cwnd_plot = []
        ssthresh_plot = []
        for d in data:
            cwnd_plot.append((float(d['time']) - first_ack, int(d['cwnd'])))
            # ssthresh_plot.append((float(d['time']) - first_ack, int(d['ssthresh'])))

        xs, ys = zip(*cwnd_plot)
        plt.plot(xs, ys, label='cwnd')

        # TODO:
        # Parse BDP from file and plot it
        # For LEO we know that is 417 packets

        left, right = plt.xlim()
        plt.plot((left, right), (417, 417), label='BDP')

        plt.xlim(left=-0.005)
        plt.ylim(bottom=0)
        plt.legend(bbox_to_anchor=(1, -.05))

        plt.xlabel('Time (s)')
        plt.ylabel('CWND size (packets)')
        
        if not savepath:
            savepath = '.'
        if not file_type:
            file_type = 'png'
        savepath = os.path.join(savepath, f'cwnd.{file_type}')

        print(f'Saving {savepath}')
        plt.savefig(savepath, bbox_inches='tight')


if __name__ == '__main__':
    if len(sys.argv) > 1:
        parser = argparse.ArgumentParser()
        
        parser.add_argument('--save_path', help='', default='plt')
        parser.add_argument('--file_type', default='png')

        parser.add_argument('--kern_log_path')

        args = parser.parse_args()

        data = get_cwnd_info(kern_log_path=args.kern_log_path)
        plot_cwnd(data, savepath=args.save_path, file_type=args.file_type)
    else:
        data = get_cwnd_info('/vagrant/logs/test/kern.log')
        plot_cwnd(data, savepath='plt', file_type='png')


