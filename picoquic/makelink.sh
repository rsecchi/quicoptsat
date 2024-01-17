

# sudo tc qdisc add dev lo root handle 1: htb default 12
# sudo tc class add dev lo parent 1: classid 1:1 htb rate 1mbit
# sudo tc class add dev lo parent 1:1 classid 1:12 htb rate 1mbit ceil 1mbit
# sudo tc qdisc add dev lo parent 1:12 netem limit 100

sudo tc qdisc del dev lo root




#sudo tc qdisc add dev lo root handle 1: prio
#sudo tc filter add dev lo protocol ip parent 1: prio 1 u32 match ip src 127.0.0.1 flowid 1:1
#sudo tc filter add dev lo protocol ip parent 1: prio 1 \
#	u32 match ip sport 4432 0xffff flowid 1:1
#sudo tc qdisc add dev lo parent 1:0 \
#	netem rate 5Mbit delay 500ms


sudo tc qdisc add dev lo root netem limit 100 rate 5Mbit delay 500ms

