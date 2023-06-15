import socket
import sys
import time

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server.bind((sys.argv[1], 80))

server.listen()

handle, _ = server.accept()

transfer_size_kb = sys.argv[2]

handle.sendall(b'C'*transfer_size_kb)

# time.sleep(3)

# handle.sendall(b'Z'*1_800_000)

# time.sleep(3)

handle.close()
