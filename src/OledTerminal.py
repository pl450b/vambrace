from luma.core.interface.serial import i2c
from luma.core.render import canvas
from luma.oled.device import ssd1306

import time

class OledTerminal:
    def __init__(self):
        self.entries = ["", "", "", "", "", "", "", ""]
        self.line_map = [10, 20, 30, 40, 50, 60, 70]
        self.serial = i2c(port=1, address=0x3C)
        self.device = ssd1306(self.serial, rotate=0)

    def new_line(self, text):
        self.entries.append(text)
        recent_entries = self.entries[-7:]
        
        with canvas(self.device) as draw:
            draw.rectangle((0, 0, 127, 10), outline="white", fill="white")
            draw.text((0, 0), "SYSTEM", fill="black")
            for i in range(7):
                draw.text((0, self.line_map[i]), recent_entries[6-i], fill="white")

if __name__ == "__main__":

    term1 = OledTerminal()
    count = 0
    while True:
        term1.new_line(f"test {count}")
        count += 1
        time.sleep(0.2)

