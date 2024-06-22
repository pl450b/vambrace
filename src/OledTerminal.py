from luma.core.interface.serial import i2c
from luma.core.render import canvas
from luma.oled.device import ssd1306

import time

class OledTerminal:
    # Class variables
    serial = i2c(port=1, address=0x3C)
    device = ssd1306(serial, rotate=0)
    
    def __init__(self, num_lines=7, line_height=10):
        # Setup of object variables
        self.num_lines = num_lines
        self.entries = [""] * self.num_lines
        self.line_height = line_height     

    def set_header(self, text):
        with canvas(self.device) as draw:
            draw.rectangle((0, 0, 127, self.line_height), outline="white", fill="white")
            draw.text((0, 0), text, fill="black")

    def new_line(self, text):
        self.entries.append(text)
        recent_entries = self.entries[-self.num_lines:]
        
        with canvas(self.device) as draw:
            for i, line in enumerate(recent_entries):
                draw.text((0, self.line_height*(i+1)), line, fill="white")

if __name__ == "__main__":
    term1 = OledTerminal()
    count = 0

    term1.set_header("SYSTEM")
    time.sleep(2)
    while True:
        term1.new_line(f"test {count}")
        count += 1
        time.sleep(0.2)
        term1.set_header("TESTER")
        time.sleep(0.2)

