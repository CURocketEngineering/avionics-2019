import mraa

uart = mraa.Uart(0)
uart.setBaudRate(9600)

def init():
    uart.writeStr('INIT\n')

def deinit():
    uart.writeStr('DEINIT\n')

def read():
    if uart.dataAvailable():
        return uart.readStr(1)
    else:
        return None

def write(data):
    uart.writeStr(data + '\n')
