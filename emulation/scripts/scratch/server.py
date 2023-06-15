import socket
import sys
import time

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server.bind((sys.argv[1], 80))

server.listen()

handle, _ = server.accept()

handle.sendall(b'C'*1_800_000)

time.sleep(3)

handle.sendall(b'Z'*1_800_000)

time.sleep(3)

handle.close()
