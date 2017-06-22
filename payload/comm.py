import mraa

uart = mraa.Uart(2)
uart.setBaudRate(9600)

class Telemetry:
    def __init__(self, acc_x, acc_y, acc_z, bar_p, bar_alt):
        self.acc_x = acc_x
        self.acc_y = acc_y
        self.acc_z = acc_z
        self.bar_p = bar_p
        self.bar_alt = bar_alt

def set_state(state):
    uart.writeStr(state)

def poll():
    global state, telemetry

    while uart.dataAvailable():
        data_type = uart.readStr(1)

        if data_type == 's':
            state = uart.readStr(1)
        elif data_type == 'u':
            data = [0,0,0,0,0,0,0,0,0,0]
            uart.read(data)
            telemetry = Telemetry((data[0] << 8) | data[1], (data[2] << 8) | data[3], (data[4] << 8) | data[5], (data[6] << 8) | data[7], (data[8] << 8) | data[9])

def get_state():
    return state

def get_telemetry():
    return telemetry
