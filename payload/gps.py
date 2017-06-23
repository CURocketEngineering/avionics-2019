from __future__ import division, print_function

import struct

import mraa

i2c = mraa.I2c(0)

class Datum(object):
    def __init__(self, lat, lon):
        self.lat = lat
        self.lon = lon

def read():
    i2c.address(0x13)

    data = [0,0,0,0,0,0,0,0]
    i2c.read(data)

    return Datum(*struct.unpack('ff', bytes(data)))
