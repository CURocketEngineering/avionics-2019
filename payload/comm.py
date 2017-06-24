from __future__ import division, print_function

import struct

import mraa

uart = mraa.Uart('/dev/ttyMFD2')
uart.setBaudRate(9600)

class Telemetry(object):
    def __init__(self, acc_x, acc_y, acc_z, bar_p, bar_alt):
        self.acc_x = acc_x
        self.acc_y = acc_y
        self.acc_z = acc_z
        self.bar_p = bar_p
        self.bar_alt = bar_alt

state = None
telemetry = Telemetry(0, 0, 0, 0, 0)

def set_state(state):
    uart.writeStr(state)

def poll():
    global state, telemetry

    while uart.dataAvailable():
        data_type = uart.readStr(1)

        if data_type == 's':
            state = uart.readStr(1)
        elif data_type == 'u':
            data = uart.read(20)
            telemetry = Telemetry(*struct.unpack('fffff', bytes(data)))

def get_state():
    return state

def get_telemetry():
    return telemetry
