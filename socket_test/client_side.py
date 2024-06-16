import socket

# Setup connection parameters (ip/port pair, packet format)
connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# Connect to my server
connection.connect(("vambrace.flynn.net", 5000))

while True:
    print(connection.recv(1024).decode("utf-8"))
