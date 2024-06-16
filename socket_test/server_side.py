import socket
import time

connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
connection.bind(('', 5000))
connection.listen(5)
print("Server Running....")

def send_data(message):
    print(f"Sending {message}")
    clientsocket.send(bytes(message, "utf-8"))

if __name__ == "__main__":
    clientsocket, address = connection.accept()
    print(f"New connection from {address}, socket = {clientsocket}")
    
    while True:
        send_data("Success!!!")
        time.sleep(2);
