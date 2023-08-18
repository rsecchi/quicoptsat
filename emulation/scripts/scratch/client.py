import socket
import time
import sys

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# print(client.getsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF))
# client.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 67108864)
# print(client.getsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF))

client.connect((sys.argv[1], 80))



expecting = int(sys.argv[2])

while expecting > 0:
    # 67108864
    # 1048576
    len_recvd = len(client.recv(67108864))
    expecting -= len_recvd
