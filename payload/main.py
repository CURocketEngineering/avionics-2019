#!/usr/bin/env python2
from __future__ import division, print_function

import json

import box
import timing

import comm

import gps

import accelerometer
import barometer
import magnetometer
import sound

FREQ = 10 # Hz

try:
    comm.init()

    barometer.init()
    magnetometer.init()
    sound.init('sound.pcm')

    box.init('blackbox.json')
    timing.init()

    delay = 1000/FREQ

    while True:
        acc = accelerometer.read()
        bar = barometer.read()
        mag = magnetometer.read()

        datum = gps.read()

        sound.sample()

        data = {'time': timing.get_millis(), 'sensor': {'acc': {'x': acc.x, 'y': acc.y, 'z': acc.z}, 'bar': {'p': bar.p, 'alt': bar.alt}, 'gps': {'lat': datum.lat, 'lon': datum.lon}, 'mag': {'x': mag.x, 'y': mag.y, 'z': mag.z}}}

        box.write(data)
        comm.send(data)

        timing.delay(delay)
except KeyboardInterrupt:
    pass
finally:
    try:
        sound.deinit()
    except:
        pass

    try:
        box.deinit()
    except:
        pass

    try:
        comm.deinit()
    except:
        pass
