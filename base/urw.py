## Loops through updating graphs 

import urwid as urwid
import json
import comm
import box
import time

palette = [
    ('normal', 'dark magenta', 'dark gray'),
    ('inverse', 'dark gray', 'dark magenta'),
]

height = 50 ##Change

myiter = 0
data2 = []
altArray = []
accelArray = []

def init():
    comm.init()
    global data
    data = {'time': -1, 'state': 'no_comm', 'sensors': None}
    global accelArray
    accelArray = []
    global altArray

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


# Converts integer to list containing only integer
def toList(someinteger):
    mylist = []
    mylist.append(someinteger)
    return mylist

#A loop for updating a graph with fake data
def updatefake(loop, data):
    time.sleep(2)
    global myiter
    data = [1,2,3,4,5,0,9]
    if myiter == 2:
        data = [4,6,8,1,2,0,0,0,0,10,3,5]
    elif myiter == 4:
        data = [1,4,2]
    elif myiter == 6:
        data = [3,2,5,6,8,9,5,3,5,7,8]
    myiter += 1
    for i in data:
        i = i*myiter
    newData = []
    for i in data:
        newData.append(tuple(toList(i)))
    graph = urwid.BarGraph(
        ['normal', 'inverse'],
        ['normal', 'inverse'],
        { (1,0): 'normal', },
    )
    lines = [height]
    graph.set_data(newData, 50, lines)
    loop.stop()
    loop = urwid.MainLoop(graph, palette)
    loop.set_alarm_in(0.1,updatefake)
    loop.run()

def updategetbig(loop, data):
    time.sleep(1)
    global data2
    global myiter
    myiter += 1
    data2.append(myiter)
    lines = [height]
    newData = []
    for i in data2:
        newData.append(tuple(toList(i)))
    graph = urwid.BarGraph(
        ['normal', 'inverse'],
        ['normal', 'inverse'],
        { (1,0): 'normal', },
    )
    graph.set_data(newData, 50, lines)
    loop.stop()
    loop = urwid.MainLoop(graph, palette)
    loop.set_alarm_in(0.1,updategetbig)
    loop.run()
        
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
    loop.set_alarm_in(0.1,updategetbig)
    loop.run()
    
while(True):
    #init()
    graph()

    
