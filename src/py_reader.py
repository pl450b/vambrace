from luma.core.interface.serial import i2c
from luma.core.render import canvas
from luma.oled.device import ssd1306, ssd1325, ssd1331, sh1106
from collections import deque

import time

fifo_path = "/tmp/gesture_fifo"
entry_list = ["", "", "", "", "", "", "", ""]

line_map = [0, 10, 20, 30, 40, 50, 60]

serial = i2c(port=1, address=0x3C)
device = ssd1306(serial, rotate=0)

def read_fifo():
    with open(fifo_path, 'r') as file:
        ir_data = file.read().strip()
    return ir_data

def new_line(text, entries):
    entries.append(text)
    recent_entries = entries[-7:]
    
    with canvas(device) as draw:
        for i in range(7):
            draw.text((0, line_map[i]), recent_entries[6-i], fill="white")
    time.sleep(1)

if __name__ == "__main__":
    new_line("test 1", entry_list)
    new_line("test 2", entry_list)
    new_line("test 3", entry_list)
    new_line("test 4", entry_list)
    new_line("test 5", entry_list)
    new_line("test 555555", entry_list)
    new_line("test 7", entry_list)
    new_line("test 8", entry_list)

    time.sleep(10)
