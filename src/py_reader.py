from luma.core.interface.serial import i2c
from luma.core.render import canvas
from luma.oled.device import ssd1306, ssd1325, ssd1331, sh1106

import time

fifo_path = "/tmp/gesture_fifo"

serial = i2c(port=1, address=0x3C)
device = ssd1306(serial, rotate=0)

def read_fifo():
    with open(fifo_path, 'r') as file:
        ir_data = file.read().strip()
    return ir_data

if __name__ == "__main__":

    while True:
        with canvas(device) as draw:
            ir_data = read_fifo()
            ir_data = ir_data.split('\x00')[0]
            x_index = int(int(ir_data)/1.7)
            draw.rectangle((1, 1, x_index, 62), outline="white", fill="black")
            print(int(ir_data))
