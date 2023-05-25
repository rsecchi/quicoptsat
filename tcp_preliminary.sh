#!/bin/bash

#  Script Usage
#  ./ns3 run scratch/satlink.cc -- --PrintHelp 
#satlink [Program Options] [General Arguments]
#
#Program Options:
#    --num_flows:       Number of flows [1]
#    --bandwidth:       Bottleneck bandwidth [5Mbps]
#    --delay:           Bottleneck delay (ms) [45ms]
#    --transport_prot:  Transport protocol [TcpCubic]
#    --qlen:            Queue Length [100p]
#    --mtu:             Packet size [1500]
#    --error_p:         Packet error rate [0]
#    --duration:        duration [100]
#    --data:            Number of kBs to send [0]
#    --seed:            Random numbers seed [1]
#    --offset:          Spacing between connections [0.001]
#
#General Arguments:
#    --PrintGlobals:              Print the list of globals.
#    --PrintGroups:               Print the list of groups.
#    --PrintGroup=[group]:        Print all TypeIds of group.
#    --PrintTypeIds:              Print all TypeIds.
#    --PrintAttributes=[typeid]:  Print all attributes of typeid.
#    --PrintVersion:              Print the ns-3 version.
#    --PrintHelp:                 Print this help message.
#


SCRIPT="scratch/satlink"
ARG=""

# 5 Mbps, 50ms --> BDP = 42 pkt
ARG="$ARG --num_flows=1"
ARG="$ARG --delay=50ms"
ARG="$ARG --qlen=55p"


echo "Preliminary simulations with TCP CCs"
NS3_DIR="/home/raffaello/workspace/ns-allinone-3.37/ns-3.37/"
NS3="/home/raffaello/workspace/ns-allinone-3.37/ns-3.37/ns3"

RANGE=$(seq 50 50 1000)

# Bad scenario
#ARG="$ARG --num_flows=2"
ARG="$ARG --error_p=0.02"
RANGE=$(seq 5 5 1000)



echo Standard Slow Start
TP=TcpNewReno
for i in $RANGE;
do
	CT=$($NS3 run "$SCRIPT $ARG --transport_prot=$TP --data=$i --seed=$i")
	echo $i $CT
done
#done > static_$TP.tr
cp $NS3_DIR/cwnd0.tr static_cwnd_$TP.tr
cp $NS3_DIR/rtt0.tr static_rtt_$TP.tr

echo Classical Hystart
TP=TcpCubic
for i in $RANGE;
do
	CT=$($NS3 run "$SCRIPT $ARG --transport_prot=$TP --data=$i --seed=$i")
	echo $i $CT
done > static_$TP.tr
cp $NS3_DIR/cwnd0.tr static_cwnd_$TP.tr
cp $NS3_DIR/rtt0.tr static_rtt_$TP.tr

echo IW=42
TP=TcpNewReno
ARG="$ARG --iw=42"
for i in $RANGE;
do
	CT=$($NS3 run "$SCRIPT $ARG --transport_prot=$TP --data=$i --seed=$i")
	echo $i $CT
done > static_${TP}_iw42.tr
cp $NS3_DIR/cwnd0.tr static_cwnd_${TP}_iw42.tr
cp $NS3_DIR/rtt0.tr static_rtt_${TP}_iw42.tr


