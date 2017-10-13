#!/usr/bin/env python2
# -*- coding: utf-8 -*-
from __future__ import division, print_function

import json
import sys

import box
import comm
import term

states = {'h': 'Idle', 't': 'Test', 'p': 'Pass', 'f': 'Fail', 'a': 'Arm', 'i': 'Ignite', 'b': 'Burn', 'c': 'Coast', 'd': 'Apogee', 'w': 'Wait', 'e': 'Eject', 'l': 'Fall', 'r': 'Recover'}

inited = False

try:
    comm.init()
    term.init()

    box.init('blackbox.json')

    data = {'time': -1, 'state': '-', 'sensor': {'acc': {'x': -1, 'y': -1, 'z': -1}, 'bar': {'p': -1, 'alt': -1}, 'gps': {'lat': -1, 'lon': -1}}}

    while True:
        try:
            cmd = sys.stdin.read(1)
            comm.send(cmd)
        except IOError:
            pass

        message = comm.read()

        if message:
            data['time'] = message['time']

            if message['type'] == 's':
                data['state'] = message['state']
            elif message['type'] == 'u':
                data['sensor'] = message['sensor']

            box.write(data)

        print('\33[2J')

        if data['state'] != '-':
            print('[{}] State: {} ∙ Acceleration: {} g ∙ Altitude: {} ft ∙ GPS: {}° {}°'.format(data['time'], states[data['state']], data['sensor']['acc']['z'], data['sensor']['bar']['alt'], data['sensor']['gps']['lat'], data['sensor']['gps']['lon']))
        else:
            print('Waiting on main computer...')
except KeyboardInterrupt:
    pass
finally:
    try:
        box.deinit()
    except:
        pass

    try:
        term.deinit()
    except:
        pass
