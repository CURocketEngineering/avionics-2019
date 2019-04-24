#!/usr/bin/env python2
# -*- coding: utf-8 -*-
from __future__ import division, print_function

import json
import time
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

    data = {'time': 0, 'state': 'no_comm', 'sensors': None}
    count = 0

    while True:
        try:
            cmd = sys.stdin.read(1)

            if cmd == 't':
                comm.send({'command': 'test'})
                print ('sent command')
            elif cmd == 'a':
                comm.send({'command': 'arm'})
                print ('arm')
            elif cmd == 'd':
                comm.send({'command': 'disarm'})
            elif cmd == 'i':
                comm.send({'command': 'ignite'})
                print ('sent ignite')
            elif cmd == 'b':
                comm.send({'command': 'abort'})
            elif cmd == 'p':
                comm.send({'command': 'pass'})
            elif cmd == 'f':
                comm.send({'command': 'fail'})
            else:
                comm.send({'command': 'none'})
        except IOError:
            comm.send({'command': 'none'})

        message = comm.read()

        if message:
            count = 0

            data['time'] = message['time']

            if message['type'] == 'state':
                data['state'] = message['state']
            elif message['type'] == 'telemetry':
                data['sensors'] = message['sensors']

            box.write(data)
        else:
            count += 1

            if count > 1000:
                data['state'] = 'no_comm'

        if data['sensors']:
            print('[{}] State: {} ∙ Acceleration: {} g ∙ Altitude: {} ft ∙ GPS: {}° {}°     '.format(data['time']/1000, states[data['state']], data['sensors']['acc']['z'], data['sensors']['bar']['alt'], data['sensors']['gps']['lat'], data['sensors']['gps']['lon']), end='\r')
        else:
            print('[{}] State: {}                                                           '.format(data['time']/1000, states[data['state']]), end='\r')
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
