import socket
import time
import sys

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

client.connect((sys.argv[1], 80))

while True:
    len_recvd = client.recv(2048)
