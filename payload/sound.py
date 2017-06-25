from __future__ import division, print_function

import struct

import mraa

signal = mraa.Aio(3)

signal.setBit(12)

def init(filename):
    global output

    output = open(filename, 'wb')

def deinit():
    output.close()

def sample():
    output.write(struct.pack('H', signal.read()))
