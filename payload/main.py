#!/usr/bin/env python3
import json

import box
import time

import radio

import comm

import gps

import accelerometer
import barometer
import magnetometer

NWS_ALT = 30.20
FREQ = 10 # Hz

try:
    comm.init()
    gps.init()
    radio.init()

    barometer.init(NWS_ALT)

    box.init()
    time.init()

    delay = 1/FREQ

    while True:
        cmd = radio.read()

        state = comm.get_state()
        telemetry = comm.get_telemetry()

        acc = accelerometer.read()
        bar = barometer.read()
        datum = gps.read()
        mag = magnetometer.read()

        data = json.dumps({'time': time.get_millis(), 'payload': {'sensor': {'acc': {'x': acc.x, 'y': acc.y, 'z': acc.z}, 'bar': {'p': bar.p, 'alt': bar.alt}, 'gps': {'lat': datum.lat, 'lon': datum.lon, 'alt': datum.alt}, 'mag': {'x': mag.x, 'y': mag.y, 'z': mag.z}}}, 'main': {'state': main.state, 'sensor': {'acc': {'x': acc.x, 'y': acc.y, 'z': acc.z}, 'bar': {'p': bar.p, 'alt': bar.alt}}}})

        box.write(data)
        radio.send(data)

        time.delay(delay)
except KeyboardInterrupt:
    pass
