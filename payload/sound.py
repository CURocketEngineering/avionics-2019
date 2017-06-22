import struct

import mraa

signal = mraa.Aio(3)

signal.setBit(12)

output = None

def init(filename):
    output = open(filename, 'wb')

def deinit():
    output.close()

def sample():
    output.write(struct.pack('H', signal.read()))
