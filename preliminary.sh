#!/bin/bash

#tcp-variants-comparison [Program Options] [General Arguments]

#Program Options:
#    --transport_prot:    Transport protocol to use: TcpNewReno, TcpLinuxReno,
#      TcpHybla, TcpHighSpeed, TcpHtcp, TcpVegas, TcpScalable, TcpVeno, TcpBic,
#      TcpYeah, TcpIllinois, TcpWestwood, TcpWestwoodPlus, TcpLedbat, TcpLp,
#      TcpDctcp, TcpCubic, TcpBbr [TcpWestwood]
#    --error_p:           Packet error rate [0]
#    --bandwidth:         Bottleneck bandwidth [2Mbps]
#    --delay:             Bottleneck delay [0.01ms]
#    --access_bandwidth:  Access link bandwidth [10Mbps]
#    --access_delay:      Access link delay [45ms]
#    --tracing:           Flag to enable/disable tracing [false]
#    --prefix_name:       Prefix of output trace file [TcpVariantsComparison]
#    --data:              Number of Megabytes of data to transmit [0]
#    --mtu:               Size of IP packets to send in bytes [400]
#    --num_flows:         Number of flows [1]
#    --duration:          Time to allow flows to run in seconds [100]
#    --run:               Run index (for setting repeatable seeds) [0]
#    --flow_monitor:      Enable flow monitor [false]
#    --pcap_tracing:      Enable or disable PCAP tracing [false]
#    --queue_disc_type:   Queue disc type for gateway (e.g. ns3::CoDelQueueDisc) [ns3::PfifoFastQueueDisc]
#    --sack:              Enable or disable SACK option [true]
#    --recovery:          Recovery algorithm type to use (e.g., ns3::TcpPrrRecovery [ns3::TcpClassicRecovery]
#
#General Arguments:
#    --PrintGlobals:              Print the list of globals.
#    --PrintGroups:               Print the list of groups.
#    --PrintGroup=[group]:        Print all TypeIds of group.
#    --PrintTypeIds:              Print all TypeIds.
#    --PrintAttributes=[typeid]:  Print all attributes of typeid.
#    --PrintVersion:              Print the ns-3 version.
#    --PrintHelp:                 Print this help message.


SCRIPT="scratch/quic-variants-comparison"
ARG=""

ARG="$ARG --duration=25"
#ARG="$ARG --tracing=true"
#ARG="$ARG --mtu=1200"
ARG="$ARG --num_flows=3"
ARG="$ARG --data=1"


echo "Preliminary simulations with TCP CCs"
NS3="/home/raffaello/workspace/ns-allinone-3.37/ns-3.37/ns3"
OUT=$(pwd)/traces

for TP in TcpNewReno;
do
	echo $TP
	rm ${OUT}_$TP/*

	$NS3 run "$SCRIPT $ARG --transport_prot=$TP --prefix_name=$TP" --cwd ${OUT}_$TP
done



