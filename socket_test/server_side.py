import socket
import time

# Setup connection parameters (ip/port pair, packet format)
connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
# Bind to any ip address on port 5000
connection.bind(('', 5000))
connection.listen(5) # allows server to accept connections (only 5 failed attempts allowed)
print("Server Running....")

# make an actuall connection 
clientsocket, address = connection.accept()
print(f"New connection from {address}, socket = {clientsocket}")

while True:
    clientsocket.send(bytes("This is a test from thhtht Wes", "utf-8")) # send message
    time.sleep(2);
