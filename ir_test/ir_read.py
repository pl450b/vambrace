fifo_path = "/tmp/gesture_fifo"

while True:
    data = open(fifo_path, 'r')
    if(data):
        print("yes data")
    else:
        print("no data")
    data.close()


        # ir_data = data.read().strip()


