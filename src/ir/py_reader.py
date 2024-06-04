import os
import time 

fifo_path = "/tmp/gesture_fifo"

def read_fifo():
    with open(fifo, 'r') as file:
        ir_data = file.read().strip()
    return ir_data

if __name__ == "__main__":
    while True:
        ir_data = read_fifo()
        print(ir_data)
