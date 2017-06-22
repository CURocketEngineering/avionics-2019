#!/usr/bin/env python3
import json

import box
import timing

import radio

import comm

import gps

import accelerometer
import barometer
import magnetometer
import sound

FREQ = 10 # Hz

try:
    comm.init()
    gps.init()
    radio.init()

    barometer.init()
    magnetometer.init()
    sound.init()

    box.init('blackbox.json')
    timing.init()

    delay = 1000/FREQ

    while True:
        cmd = radio.read()
        if cmd:
            comm.set_state(cmd)

        comm.poll()

        state = comm.get_state()
        telemetry = comm.get_telemetry()

        acc = accelerometer.read()
        bar = barometer.read()
        mag = magnetometer.read()

        gps.poll()

        datum = gps.get_datum()

        sound.sample()

        data = json.dumps({'time': timing.get_millis(), 'payload': {'sensor': {'acc': {'x': acc.x, 'y': acc.y, 'z': acc.z}, 'bar': {'p': bar.p, 'alt': bar.alt}, 'gps': {'lat': datum.lat, 'lon': datum.lon, 'alt': datum.alt}, 'mag': {'x': mag.x, 'y': mag.y, 'z': mag.z}}}, 'main': {'state': main.state, 'sensor': {'acc': {'x': telemetry.acc_x, 'y': telemetry.acc_y, 'z': telemetry.acc_z}, 'bar': {'p': telemetry.bar_p, 'alt': telemetry.bar_alt}}}})

        box.write(data)
        radio.send(data)

        timing.delay(delay)
except KeyboardInterrupt:
    pass
finally:
    sound.deinit()

    box.deinit()

    radio.deinit()
