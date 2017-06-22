import mraa

i2c = mraa.I2c(0)

class MagnetometerReading(object):
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

def init():
    i2c.address(0x0E)
    i2c.writeReg(0x11, 0x80)
    i2c.writeReg(0x10, 0x01)

def read():
    i2c.address(0x0E)
    return MagnetometerReading((i2c.readReg(0x01) << 8) | i2c.readReg(0x02), (i2c.readReg(0x03) << 8) | i2c.readReg(0x04), (i2c.readReg(0x05) << 8) | i2c.readReg(0x06))
