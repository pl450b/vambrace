from .wrist_screen import WristScreen

import time
import threading

fifo_path = "/tmp/gesture_fifo"

def read_ir(path, lock, screen):
    while True:
        file = open(path, 'r')
        lock.acquire()
        ir_data = file.read().strip()
        if(ir_data == "999"):
            lock.release()
        screen.new_line("SYSTEM", ir_data)

screen_lock = threading.Lock()

if __name__ == "__main__":
    sys_screen = WristScreen()
    test_screen = WristScreen()

    thread1 = threading.Thread(target=read_ir, args=(fifo_path, screen_lock, sys_screen))

    while True:
        screen_lock.acquire()
        test_screen.new_line("TEST", "test from wes")
        screen_lock.release()
        time.sleep(3)



