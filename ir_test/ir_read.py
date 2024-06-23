fifo_path = "/tmp/gesture_fifo"

while True:
    try:
        data = open(fifo_path, 'r')
        if(data):
            print("yes data")
        else:
            print("no data")
    except:
        print("really no data")
    finally:
        data.close()


        # ir_data = data.read().strip()


