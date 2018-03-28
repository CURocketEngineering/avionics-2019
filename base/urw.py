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
    comm.init()
    global data = {'time': -1, 'state': 'no_comm', 'sensors': None}
    global accelArray = []
    global altArray = []

def update():
    global accelArray
    global altArray
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

##Just altitude graph for now
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
                             
altArray = [(1,),(2,),(3,),(4,),(0,),(3,),(6,)] #temp until fake data parse
#init()
while(True):
    #update()
    graph()
