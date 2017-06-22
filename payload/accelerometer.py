import mraa

aio_x = mraa.Aio(0)
aio_y = mraa.Aio(1)
aio_z = mraa.Aio(2)

class AccelerometerReading(object):
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

def read():
    return AccelerometerReading(aio_x.read(), aio_y.read(), aio_z.read())
