## Loops through updating graphs 

import urwid as urwid
import json
import comm
import box

def init():
    states = {'no_comm': 'Disconnected', 'idle': 'Idle', 'test': 'Test', 'pass': 'Pass', 'fail': 'Fail', 'arm': 'Arm', 'ignite': 'Ignite', 'burn': 'Burn', 'coast': 'Coast', 'apogee': 'Apogee', 'wait': 'Wait', 'eject': 'Eject', 'fall': 'Fall', 'recover': 'Recover'}
    comm.init()
    box.init('blackbox.json')
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

##
def graph():
    x = [1,2,3,4,5]
    y = [1,10,4,8,5]
    graph = urwid.BarGraph('dark magenta')
    graph.set_data(x,100)
    graph.render()

init()
graph()
while(True):
    pass
    update()
    graph()
