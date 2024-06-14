from luma.core.interface.serial import i2c
from luma.core.render import canvas
from luma.oled.device import ssd1306, ssd1325, ssd1331, sh1106

import time

fifo_path = "/tmp/gesture_fifo"
entry_list = []

serial = i2c(port=1, address=0x3C)
device = ssd1306(serial, rotate=0)

def read_fifo():
    with open(fifo_path, 'r') as file:
        ir_data = file.read().strip()
    return ir_data

def new_line(text, entries):
    entries.append(text)
    recent_entries = entries[-4:]
    for i in range(4):
        current_line = recent_entries[i]
        with canvas(device) as draw:
            draw.text((1,64-i*10), current_line, fill="white")
    time.sleep(1)

if __name__ == "__main__":
    new_line("test 1", entry_list)
    new_line("Test_2", entry_list)
    new_line("testtttt 3", entry_list)
    new_line("test  test  4", entry_list);

          
