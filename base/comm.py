from __future__ import division, print_function

import serial
import struct

def init():
    global comm
    comm = serial.Serial('/dev/ttyACM0', 9600, timeout=1)

def read():
    header = comm.read(5)

    if not header:
        return {}

    data_time, data_type = struct.unpack('fc', bytes(header))

    if data_type == 's':
        state = struct.unpack('c', bytes(comm.read(1)))[0]
        return {'time': data_time, 'type': 's', 'state': state}
    elif data_type == 'u':
        acc_x, acc_y, acc_z, bar_p, bar_alt, gps_lat, gps_lon = struct.unpack('fffffff', bytes(comm.read(28)))
        return {'time': data_time, 'type': 'u', 'sensor': {'acc': {'x': acc_x, 'y': acc_y, 'z': acc_z}, 'bar': {'p': bar_p, 'alt': bar_alt}, 'gps': {'lat': gps_lat, 'lon': gps_lon}}}
    else:
        return {'time': data_time, 'type': data_type}

def send(cmd):
    comm.write(cmd)
