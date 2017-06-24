from __future__ import division, print_function

import serial

def init():
    global comm
    comm = serial.Serial('/dev/ttyACM0', 9600)

def read():
    return comm.readline()

def send(cmd):
    comm.write(cmd + '\n')
