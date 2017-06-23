#!/usr/bin/env python2
# -*- coding: utf-8 -*-
from __future__ import print_function

import json
import sys

import comm
import term

states = {'h': 'Idle', 't': 'Test', 'p': 'Pass', 'f': 'Fail', 'a': 'Arm', 'i': 'Ignite', 'b': 'Burn', 'c': 'Coast', 'd': 'Apogee', 'w': 'Wait', 'e': 'Eject', 'l': 'Fall', 'r': 'Recover'}

inited = False

try:
    comm.init()
    term.init()

    while True:
        try:
            cmd = sys.stdin.read(1)
            comm.send(cmd)
        except IOError:
            pass

        line = comm.read()

        if line == 'INIT':
            inited = True
        elif line == 'DEINIT':
            inited = False

        data = json.loads(line)

        print('\33[2J')

        if inited:
            print('[{}] State: {} • Acceleration: {} g • Altitude: {} ft • GPS: {} {}'.format(connected, data['time'], states[data['main']['state']], data['main']['sensor']['acc']['z'] if data['main']['state'] != 'e' else data['payload']['sensor']['acc']['z'], data['main']['sensor']['bar']['alt'] if data['main']['state'] != 'e' else data['payload']['sensor']['bar']['alt'], data['payload']['sensor']['gps']['lat'], data['payload']['sensor']['gps']['lon']))
        else:
            print('Waiting on init...')
except KeyboardInterrupt:
    pass
finally:
    term.deinit()
