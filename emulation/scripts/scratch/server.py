import socket
import sys
import time

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# print(server.getsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF))

# 16777216
# 33554432 
# 67108864
# 134217728
# 268435456
# 536870912
# 1073741824
# -------
# 2147483648

# server.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 67108864)

# print(server.getsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF))

server.bind((sys.argv[1], 80))

server.listen()

while True:
    handle, _ = server.accept()

    transfer_size_kb = int(sys.argv[2])

    handle.sendall(b'C'*transfer_size_kb)

    # time.sleep(3)

    # handle.sendall(b'Z'*1_800_000)

    # time.sleep(3)

    handle.close()
