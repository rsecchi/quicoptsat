import socket
import time
import sys

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

client.connect((sys.argv[1], 80))

expecting = int(sys.argv[2])

while expecting > 0:
    len_recvd = len(client.recv(2048))
    expecting -= len_recvd
