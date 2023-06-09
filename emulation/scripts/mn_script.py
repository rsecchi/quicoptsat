from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import CPULimitedHost
from mininet.link import TCLink
from mininet.util import dumpNodeConnections
from mininet.log import setLogLevel
from mininet.cli import CLI

import os
import sys
import math
import json
import time
import datetime

precise_time_str = "%y-%m-%d-%H:%M:%S:%f"

def _calculate_bdp(bw, RTT):
	'''
		Returns BDP in MTU sized packets, default MTU=1500
		@bw -> badwidth in Mbps
		@RTT -> round trip time in ms
	'''
	bdp = bw * 1000 # kbps
	bdp /= 8 #kBps
	bdp *= RTT # Bytes
	bdp = math.ceil(bdp / 1500.0) # MTU sized packets
	return int(bdp)


def changeLinkProps(ep1, ep2, in_bw, RTT, logger, out_bw=-1, loss=None):

	msg = 'changing BW %s RTT %s loss %s %s ' % (in_bw, RTT, loss, datetime.datetime.now().strftime(precise_time_str))
	print(msg)
	logger.append(msg)
	link = ep1.connectionsTo(ep2)
	link_prop = {'delay': '%sms' % (RTT / 2)}
	if in_bw:
		link_prop['bw'] = in_bw
		bdp = _calculate_bdp(in_bw, RTT)
		link_prop['max_queue_size'] = bdp
	if loss:
		link_prop['loss'] = loss
	link[0][0].config(**link_prop)

	egress_link_prop = {'delay': '%sms' % (RTT / 2)}
	if out_bw and out_bw != -1:
		egress_link_prop['bw'] = out_bw
		bdp = _calculate_bdp(out_bw, RTT)
		egress_link_prop['max_queue_size'] = bdp
	elif in_bw:
		egress_link_prop['bw'] = in_bw
		bdp = _calculate_bdp(in_bw, RTT)
		egress_link_prop['max_queue_size'] = bdp
	link[0][1].config(**egress_link_prop)


def config_bw(conf_file, ep1, ep2, logger, ignore_link_loss):
	delta_time = 0
	with open(conf_file) as f:
		conf = json.load(f)

	repeat = conf.get('repeat')
	if repeat:
		repeat = eval(repeat)

	if repeat:
		total_time = 0
		pause = conf['pause']
		while(total_time < conf['duration']):
			for entry in conf['changes']:
				wait_for = entry['time'] - delta_time
				time.sleep(wait_for)
				total_time += wait_for

				if total_time > conf['duration']: # Check if we should exit
					return

				loss = None
				if not ignore_link_loss:
					loss = entry.get('loss')
				# Change BW
				changeLinkProps(ep1, ep2, entry['bw'], entry['rtt'], logger, loss=loss)
				delta_time = entry['time']
				
			time.sleep(pause) # wait for PAUSE seconds before looping again
			total_time += pause
			# current_time = (entry['time'] + conf['repeat']) * cycles
			
			delta_time = 0
			
	else:
		for entry in conf['changes']:
			wait_for = entry['time'] - delta_time
			time.sleep(wait_for)
			# Change BW
			changeLinkProps(ep1, ep2, entry['bw'], entry['rtt'], logger)
			delta_time = entry['time']

class DumbbellTopo( Topo ):
    "Dumbbell topology with n hosts."

    _bw = None
    _RTT = None

    _hosts = 0

    def build( self, n=2 ):
        if n != 2:
            print("Building topo with %s nodes" % n)
        s1 = self.addSwitch( 's1' )
        s2 = self.addSwitch( 's2' )
        hosts = []
        for h in range(n):
            host = self.addHost( 'h%s' % (h + 1))
            hosts += [host]

        # 10 Mbps, 5ms delay, 2% loss, 1000 packet queue
        bw = 50 #Mbps
        global bw_init
        bw_init = self._bw = bw
        RTT = self._RTT = 70 #ms
        bdp = _calculate_bdp(bw, RTT)
        print(bdp) 

        # Leaving non-bottleneck links to run at maximum capacity with default parameters
        for i in range(len(hosts)):
            self.addLink(hosts[i], s1 if i % 2 == 0 else s2)#, bw=bw, delay='%dms' % delay, max_queue_size=bdp)

        self.addLink( s1, s2, bw=bw, delay='%dms' % (RTT/2), max_queue_size=bdp)


def doSimulation(log_root=None, cong_alg=None,
                     network_model_file=None, mpd_location=None, dash_alg=None,
                      ignore_link_loss=None, clients=1, start_seeds_file='', rtts=None,
                        background_traffic_path=None):
    "Create network and run simple performance test"

    # Create Topology
    topo = DumbbellTopo(n=(clients * 2))
    net = Mininet( topo=topo,
               host=CPULimitedHost, link=TCLink)
    
    net.start()

    s1, s2 = net.get('s1', 's2')
    config_bw('/vagrant/network_models/links/DSL.json', s1, s2, [], None)

    server, client = net.get('h1', 'h2')
    net.pingAll()

    if cong_alg:
        print("Enabling " + cong_alg + " at the server...")
        server.cmd('sudo bash /vagrant/scripts/enable_' + cong_alg + '.sh')
        cc_alg = server.cmd('sudo sysctl net.ipv4.tcp_congestion_control')
        print('cc alg', cc_alg)

    for host in [server, client]:
        host.cmd('ethtool -K ' + str(host.intf()) + ' gso off')
        host.cmd('ethtool --offload ' + str(host.intf()) + ' tso off')
    
    # Do Simulation

    pcap_tmp_path = '/home/vagrant/tmp'
    save_root = '/vagrant/logs/DSL/%s' % cong_alg

    # Clear previous kernel logs
    server.cmd('echo "" > /var/log/kern.log')
    print("Kernel logs cleared")

    sender_pcap_path = os.path.join(pcap_tmp_path, 'server.pcap')
    server_pcap = server.popen('tcpdump -s 200 -w %s -z gzip' % sender_pcap_path)
    receiver_pcap_path = os.path.join(pcap_tmp_path, 'client.pcap')
    client_pcap = client.popen('tcpdump -s 200 -w %s -z gzip' % receiver_pcap_path)

    print('Running')
    server.cmd('sudo python3 /vagrant/scripts/scratch/server.py %s&' % server.IP())
    server_pid = server.cmd("echo $!")
    client.cmd('sudo python3 /vagrant/scripts/scratch/client.py %s&' % server.IP())


    server.cmd('wait %s' % server_pid)
    print('Done')
    # copy kernel logs
    os.system('sudo cp /var/log/kern.log %s/' % (save_root))

    server_pcap.terminate()
    client_pcap.terminate()

    print("Copying packet captuires to %s" % save_root)
    os.system("mv /home/vagrant/tmp/*pcap %s" % save_root)
    net.stop()


if __name__ == '__main__':
    doSimulation(cong_alg='cubic_cr')

