#!/bin/bash

echo TEST1

#tcp-variants-comparison [Program Options] [General Arguments]

# Plot to show how IW impacts a set for different-sized transfers, but ignoring
# effects on CC, see later 

SCRIPT="scratch/satlink.cc"
NS3="/home/raffaello/workspace/ns-allinone-3.37/ns-3.37/ns3"
NS3_DIR="/home/raffaello/workspace/ns-allinone-3.37/ns-3.37/"

IW_LEO_F=400
IW_LEO_H=200
Q_LEO=176

IW_GEO_F=2000
IW_GEO_H=100
Q_GEO=900


$NS3 run $SCRIPT -- --bandwidth=100Mbps --delay=25ms --iw=10 --data=1000 --qlen=400p
mv $NS3_DIR/cwnd0.tr cwnd_iw10.tr
mv $NS3_DIR/bottln_queue.tr bq_iw10.tr

$NS3 run $SCRIPT -- --bandwidth=100Mbps --delay=25ms --iw=200 --data=1000 --qlen=400p
mv $NS3_DIR/cwnd0.tr cwnd_iw20.tr
mv $NS3_DIR/bottln_queue.tr bq_iw20.tr

$NS3 run $SCRIPT -- --bandwidth=100Mbps --delay=25ms --iw=400 --data=1000 --qlen=400p
mv $NS3_DIR/cwnd0.tr cwnd_iw200.tr
mv $NS3_DIR/bottln_queue.tr bq_iw200.tr



$NS3 run $SCRIPT -- --bandwidth=100Mbps --delay=25ms --iw=10 --data=1000 --qlen=400p
mv $NS3_DIR/cwnd0.tr cwnd_iw10.tr
mv $NS3_DIR/bottln_queue.tr bq_iw10.tr

$NS3 run $SCRIPT -- --bandwidth=100Mbps --delay=25ms --iw=200 --data=1000 --qlen=400p
mv $NS3_DIR/cwnd0.tr cwnd_iw20.tr
mv $NS3_DIR/bottln_queue.tr bq_iw20.tr

$NS3 run $SCRIPT -- --bandwidth=100Mbps --delay=25ms --iw=400 --data=1000 --qlen=400p
mv $NS3_DIR/cwnd0.tr cwnd_iw200.tr
mv $NS3_DIR/bottln_queue.tr bq_iw200.tr



