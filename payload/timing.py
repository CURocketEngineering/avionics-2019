from __future__ import division, print_function

import time

start = 0

def init():
    global start

    start = time.time()

def get_millis():
    return (time.time() - start)*1000

def delay(millis):
    time.sleep(millis/1000)
