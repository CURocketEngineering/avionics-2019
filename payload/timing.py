import time

start = 0

def init():
    start = time.time()

def get_millis():
    return (time.time() - start)*1000

def delay(millis):
    time.sleep(millis/1000)
