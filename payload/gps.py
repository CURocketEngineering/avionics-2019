from __future__ import division, print_function

import mraa

class Datum(object):
    def __init__(self, lat, lon):
        self.lat = lat
        self.lon = lon

uart = mraa.Uart('/dev/ttyMFD1')
uart.setBaudRate(9600)

datum = Datum(0, 0)

def poll():
    global datum

    while uart.dataAvailable():
        sentence = ''

        char = uart.readStr(1)
        while char != '\n':
            sentence += char

        data = sentence.split(',')

        if data[0] != '$GPRMC':
            continue

        lat = data[3]
        if data[4] == 'S':
            lat = -lat

        lon = data[5]
        if data[6] == 'W':
            lon = -lon

        datum = Datum(lat, lon)

def get_datum():
    return datum
