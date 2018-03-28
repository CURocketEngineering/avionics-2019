
import urwid
import term
import json
import comm
import box

##
def init():
    states = {'no_comm': 'Disconnected', 'idle': 'Idle', 'test': 'Test', 'pass': 'Pass', 'fail': 'Fail', 'arm': 'Arm', 'ignite': 'Ignite', 'burn': 'Burn', 'coast': 'Coast', 'apogee': 'Apogee', 'wait': 'Wait', 'eject': 'Eject', 'fall': 'Fall', 'recover': 'Recover'}
    comm.init()
    term.init()
    box.init('blackbox.json')
    data = {'time': -1, 'state': 'no_comm', 'sensors': None}
    accelArray = []
    altArray = []
    pass

##
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

        box.write(data)
        
    if data['sensors']:
        accelArray.append(states['sensors']['acc']['z'])
        altArray.append(states['sensors']['bar']['alt'])
    
    pass

##
def graph():
    #
    pass


init()
while(True):
    update()
    graph()
