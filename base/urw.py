## Loops through updating graphs 

import urwid as urwid
import json
import comm
import box

palette = [
    ('normal', 'dark magenta', 'dark gray'),
    ('inverse', 'dark gray', 'dark magenta'),
]

def init():
    states = {'no_comm': 'Disconnected', 'idle': 'Idle', 'test': 'Test', 'pass': 'Pass', 'fail': 'Fail', 'arm': 'Arm', 'ignite': 'Ignite', 'burn': 'Burn', 'coast': 'Coast', 'apogee': 'Apogee', 'wait': 'Wait', 'eject': 'Eject', 'fall': 'Fall', 'recover': 'Recover'}
    comm.init()
    data = {'time': -1, 'state': 'no_comm', 'sensors': None}
    accelArray = []
    altArray = []
    pass

def update():
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

        
    if data['sensors']:
        accelArray.append(states['sensors']['acc']['z'])
        altArray.append(states['sensors']['bar']['alt'])

    pass

##AltitudeGraph
def graph():
    altitudeGraph = urwid.BarGraph(
        ['normal', 'inverse'],
        ['normal', 'inverse'],
        { (1,0): 'normal', },
    )
    global altArray
    lines = [50]
    altitudeGraph.set_data(altArray, 50, lines)
    loop = urwid.MainLoop(altitudeGraph, palette)
    loop.run()
                             
altArray = [(1,),(2,),(3,),(4,),(0,),(3,),(6,)]
#init()
#graph()
while(True):
    #update()
    graph()
