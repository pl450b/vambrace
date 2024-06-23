import time

fifo_path = "/tmp/gesture_fifo"

while True:
    with open(fifo_path, 'r') as file:
        ir_data = file.read().strip()
        if(ir_data):
            print("Data coming in!")
        else:
            print("No data :(")
    time.sleep(1)

