#!/usr/bin/env python2
# -*- coding: utf-8 -*-
from __future__ import division, print_function

import json
import sys

import box
import comm
import term

states = {'no_comm': 'Disconnected', 'idle': 'Idle', 'test': 'Test', 'pass': 'Pass', 'fail': 'Fail', 'arm': 'Arm', 'ignite': 'Ignite', 'burn': 'Burn', 'coast': 'Coast', 'apogee': 'Apogee', 'wait': 'Wait', 'eject': 'Eject', 'fall': 'Fall', 'recover': 'Recover'}

inited = False

try:
    comm.init()
    term.init()

    box.init('blackbox.json')

    data = {'time': -1, 'state': 'no_comm', 'sensors': None}

    while True:
        try:
            cmd = sys.stdin.read(1)
            comm.send(cmd)
        except IOError:
            pass

        message = comm.read()

        if message:
            data['time'] = message['time']

            if message['type'] == 'state':
                data['state'] = message['state']
            elif message['type'] == 'telemetry':
                data['sensors'] = message['sensors']

            box.write(data)

        if data['sensors']:
            print('[{}] State: {} ∙ Acceleration: {} g ∙ Altitude: {} ft ∙ GPS: {}° {}°     '.format(data['time'], states[data['state']], data['sensors']['acc']['z'], data['sensors']['bar']['alt'], data['sensors']['gps']['lat'], data['sensors']['gps']['lon']), end='\r')
        else:
            print('[{}] State: {}                                                           '.format(data['time'], states[data['state']], end='\r')
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
