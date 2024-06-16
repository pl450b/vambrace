import socket

connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
connection.connect(("vambrace.flynn.net", 5000))

while True:
    print(connection.recv(1024).decode("utf-8"))
