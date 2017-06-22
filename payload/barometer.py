import timing

import mraa

i2c = mraa.I2c(0)

class BarometerReading:
    def __init__(self, p, alt):
        self.p = p
        self.alt = alt

def uint_to_int(uint):
    if uint & 0x8000:
        uint = -0x1000 + uint
    return uint


def init():
    global AC1, AC2, AC3, AC4, AC5, AC6, VB1, VB2, MB, MC, MD, c3, c4, b1, c5, c6, mc, md, x0, x1, x2, y0, y1, y2, p0, p1, p2

    i2c.address(0x77)

    AC1 = uint_to_int(i2c.readWordReg(0xAA))
    AC2 = uint_to_int(i2c.readWordReg(0xAC))
    AC3 = uint_to_int(i2c.readWordReg(0xAE))
    AC4 = i2c.readWordReg(0xB0)
    AC5 = i2c.readWordReg(0xB2)
    AC6 = i2c.readWordReg(0xB4)
    VB1 = uint_to_int(i2c.readWordReg(0xB6))
    VB2 = uint_to_int(i2c.readWordReg(0xB8))
    MB = uint_to_int(i2c.readWordReg(0xBA))
    MC = uint_to_int(i2c.readWordReg(0xBC))
    MD = uint_to_int(i2c.readWordReg(0xBE))

    c3 = 160.0*pow(2, -15)*AC3
    c4 = pow(10, -3) * pow(2, -15)*AC4
    b1 = pow(160, 2)*pow(2, -30)*VB1
    c5 = (pow(2, -15)/160)*AC5
    c6 = AC6
    mc = (pow(2, 11)/pow(160, 2))*MC
    md = MD/160.0
    x0 = AC1
    x1 = 160.0*pow(2, -13)*AC2
    x2 = pow(160, 2)*pow(2, -25)*VB2
    y0 = c4*pow(2, 15)
    y1 = c4*c3
    y2 = c4*b1
    p0 = (3791.0 - 8.0)/1600.0
    p1 = 1.0 - 7357.0*pow(2, -20)
    p2 = 3038.0*100.0*pow(2, -36)

def read():
    i2c.address(0x77)

    i2c.writeReg(0xF4, 0x2E)
    timing.delay(10)
    data = [0,0]
    i2c.readBytesReg(0xF6, data)

    tu = (data[0]*256.0) + data[1]

    a = c5*(tu - c6)
    T = a + (mc/(a + md))

    i2c.writeReg(0xF4, 0xF4)
    timing.delay(30)
    data = [0,0,0]
    i2c.readBytesReg(0xF6, data)

    pu = (data[0]*256.0) + data[1] + (data[2]/256.0)

    s = T - 25.0
    x = (x2*pow(s, 2)) + (x1*s) + x0
    y = (y2*pow(s, 2)) + (y1*s) + y0
    z = (pu - x) / y
    P = (p2*pow(z, 2)) + (p1*z) + p0

    alt = 145440.0*(1 - pow(P/1013.25, 1/5.255))

    return BarometerReading(P, alt)
