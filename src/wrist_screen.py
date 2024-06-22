from luma.core.interface.serial import i2c
from luma.core.render import canvas
from luma.oled.device import ssd1306

import time

class WristScreen:
    # Class variables
    serial = i2c(port=1, address=0x3C)
    device = ssd1306(serial, rotate=0)
    
    def __init__(self, num_lines=7, line_height=10):
        # Setup of object variables
        self.num_lines = num_lines
        self.entries = [""] * self.num_lines
        self.line_height = line_height     
    
    def clear(self):
        self.device.clear()

    def new_line(self, header, text):
        self.entries.append(text)
        recent_entries = self.entries[-1:self.num_lines:-1]
        
        with canvas(self.device) as draw:
            # Draw header
            draw.rectangle((0, 0, 127, self.line_height), outline="white", fill="white")
            draw.text((0, 0), header, fill="black")
            # Draw lines
            for i, line in enumerate(recent_entries):
                draw.text((0, self.line_height*(i+1)), line, fill="white")

    def device_select(self, device_list, index):
        # TODO: Fill this out
        print("not done")


if __name__ == "__main__":
    screen1 = WristScreen()
    count = 0

    while True:
        screen1.new_line("TESTER", f"test {count}")
        count += 1
        time.sleep(0.2)
