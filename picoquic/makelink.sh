

# sudo tc qdisc add dev lo root handle 1: htb default 12
# sudo tc class add dev lo parent 1: classid 1:1 htb rate 1mbit
# sudo tc class add dev lo parent 1:1 classid 1:12 htb rate 1mbit ceil 1mbit
# sudo tc qdisc add dev lo parent 1:12 netem limit 100


echo "== cleaning start =="
sudo tc qdisc del dev lo root
echo "== cleaning done =="




#sudo tc qdisc add dev lo root handle 1: prio
#sudo tc filter add dev lo protocol ip parent 1: prio 1 u32 match ip src 127.0.0.1 flowid 1:1
#sudo tc filter add dev lo protocol ip parent 1: prio 1 \
#	u32 match ip sport 4432 0xffff flowid 1:1
#sudo tc qdisc add dev lo parent 1:0 \
#	netem rate 5Mbit delay 500ms

echo "== config start =="
#sudo tc qdisc add dev lo parent root netem limit 100 rate 5Mbit delay 500ms
echo "== create parent qdisc =="
sudo tc qdisc add dev lo root handle 1: prio bands 4
echo "== create child qdisc with netem =="
sudo tc qdisc add dev lo parent 1:4 handle 40: netem delay 250ms rate 1mbit

echo "== create filter for redirecting =="
sudo tc filter add dev lo parent 1: prio 4 protocol ip u32 match ip src 127.0.0.1 match ip sport 4433 0x0000 flowid 1:4

echo "== config done =="

echo ""
echo "== qdiscs =="
sudo tc qdisc ls
echo "== filters =="
sudo tc filter ls
echo "== classes =="
sudo tc class ls
