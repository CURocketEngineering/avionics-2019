from __future__ import division, print_function

import mraa

uart = mraa.Uart(0)
uart.setBaudRate(9600)

def init():
    uart.writeStr('INIT\n')

def deinit():
    uart.writeStr('DEINIT\n')

def read():
    if uart.dataAvailable():
        return uart.readStr(2)[0]
    else:
        return None

def send(data):
    uart.writeStr(data + '\n')
