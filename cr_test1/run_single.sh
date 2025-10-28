#!/bin/bash

#tcp-variants-comparison [Program Options] [General Arguments]

# Plot to show how IW impacts a set for different-sized transfers, but ignoring
# effects on CC, see later 

SCRIPT="scratch/satlink.cc"
NS3="/home/raffaello/workspace/ns-allinone-3.39/ns-3.39/ns3"
NS3_DIR="/home/raffaello/workspace/ns-allinone-3.39/ns-3.39/"

run() {
for d in $(seq $RANGE);
do
	res=$($NS3 run $SCRIPT -- $PARAMS $TEST --data=$d)
	echo $d $res
done 
}

$NS3 run $SCRIPT -- --help
exit

######### LEO SCENARIO
# BDP = 0.625 MB
PARAMS_LEO=" \
    --bandwidth=100Mbps \
    --delay=25ms \
    --qlen=180p
"
# BDP = 6.25MB
PARAMS_GEO=" \
    --bandwidth=50Mbps \
    --delay=25ms \
    --qlen=90p
"
RANGE="5000 5000 5000"

PARAMS="$PARAMS_LEO"
TEST="--transport_prot=TcpCubicCr --iw=10 --last_cwnd=200 --last_rtt=50ms --ssthresh_reset=1.0"
run 

cp $NS3_DIR/cwnd0.tr .


exit

TEST="--transport_prot=TcpCubic --iw=10"
run > leo_cubic_iw10.tr

TEST="--transport_prot=TcpCubic --iw=200 "
run > leo_cubic_iw200.tr


PARAMS="$PARAMS_GEO"
TEST="--transport_prot=TcpCubicCr --iw=10 --last_cwnd=900 --last_rtt=500ms "
run > geo_cr_cwnd900.tr 

TEST="--transport_prot=TcpCubic --iw=10"
run > geo_cubic_iw10.tr

TEST="--transport_prot=TcpCubic --iw=900 "
run > geo_cubic_iw900.tr
