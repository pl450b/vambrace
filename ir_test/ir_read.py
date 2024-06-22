fifo_path = "/tmp/gesture_fifo"

while True:
    with open(fifo_path, 'r') as file:
        ir_data = file.read().strip()
    print(ir_data)

