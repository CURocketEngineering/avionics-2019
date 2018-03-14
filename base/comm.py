from __future__ import division, print_function

import serial
import sys
import json

def init():
    global comm
    comm = serial.Serial('/dev/ttyACM0' if len(sys.argv) != 2 else sys.argv[1], 9600, timeout=1)

def read():
    try:
        data = json.load(comm)

        return data

    except IOError:
        print('Flushing serial buffers...                                                                     ')

        comm.reset_input_buffer()
        comm.reset_output_buffer()

def send(cmd):
    comm.write(cmd)
