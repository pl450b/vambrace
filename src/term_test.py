import sys
import time
from luma.core.interface.serial import i2c
from luma.oled.device import ssd1306
from PIL import Image, ImageDraw, ImageFont

# Initialize the I2C interface and OLED display
serial = i2c(port=1, address=0x3C)
device = ssd1306(serial, width=128, height=64)

# Initialize a blank image for drawing
image = Image.new('1', (device.width, device.height))
draw = ImageDraw.Draw(image)
font = ImageFont.load_default()

# Function to update the display
def update_display(text):
    draw.rectangle((0, 0, device.width, device.height), outline=0, fill=0)
    draw.text((0, 0), text, font=font, fill=255)
    device.display(image)

# Main function to capture and display input
def main():
    print("Start typing... (Press 'ESC' to exit)")
    text = "test from wes"
    update_display(text)
    time.sleep(10)
    
if __name__ == "__main__":
    try:
        main()
    finally:
        device.clear()

