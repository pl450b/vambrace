from .wrist_screen import WristScreen
import time

if __name__ == "__main__":
    screen1 = WristScreen()
    count = 0

    while True:
        screen1.new_line("TESTER", f"test {count}")
        count += 1
        time.sleep(0.2)


