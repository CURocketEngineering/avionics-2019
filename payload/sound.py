import struct

import mraa

#signal = mraa.Aio(3)

#signal.setBit(12)

def init(filename):
    global output

    output = open(filename, 'wb')

def deinit():
    output.close()

def sample():
    return

    output.write(struct.pack('H', signal.read()))
