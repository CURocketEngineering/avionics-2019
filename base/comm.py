from __future__ import division, print_function

import serial
import sys
import json

def init():
    global comm
    comm = serial.Serial('/dev/ttyUSB0' if len(sys.argv) != 2 else sys.argv[1], 115200, timeout=1)

def read():
    try:
        try:
            data = json.loads(comm.readline())
        except ValueError:
            return None

        return data

    except IOError:
        print('Flushing serial buffers...                                                                     ')

        comm.reset_input_buffer()
        comm.reset_output_buffer()

def send(cmd):
    comm.write(json.dumps(cmd))
    comm.flush()
