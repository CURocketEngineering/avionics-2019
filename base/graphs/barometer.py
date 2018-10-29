## mpl.py - Harrison Hall - use python3
## Designed to essentially replace communications with a graphical interface
'''
TODO
* Integrate necessary code to be pulled from main
* Graphs for altitude(target height), velocity, acceleration
* Animation for rocket spin, tilt, ...
* Meters for temperature, pressure, ...
* Print data that was in comm.py in matplotlib window (GPS, ...)
* What is the json data format?
'''
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.widgets import Slider, Button, RadioButtons
import time
import math
import sys
import comm as comm
import serial
import json

PURPLE = '#522D80'

##Calculations
def calcspeed(arr_acc,arr_sec,arr_vel):
    #Calc based approach does not check for false values
    time = len(arr_sec) - 1
    dt = arr_sec[time] - arr_sec[time - 1]
    g_to_feet = 32.174
    new_vel = arr_vel[time] + (dt * (arr_acc[time] * g_to_feet))
    arr_vel.append(new_vel)

#Functions for animating altitude and acceleration
def animate(i):
    try:
        global arr_acc
        global arr_alt
        global arr_sec
        message = comm.read()
        if message:
            count = 0
            data['time'] = message['time']
            if message['type'] == 'state':
                data['state'] = message['state']
            elif message['type'] == 'telemetry':
                data['sensors'] = message['sensors']
        else:
            count += 1
            if count > 1000:
                data['state'] = 'no_comm'
                    
        if data['sensors']:
            print("Type: ",str(type(states['sensors']['acc']['z'])))
            arr_acc.append(states['sensors']['acc']['z'])
            arr_alt.append(states['sensors']['bar']['alt'])
            arr_sec.append(time.time()-start_time)
            
            plot_acc.clear()
            plot_acc.plot(arr_sec,arr_acc,color=PURPLE)
            plot_alt.clear()
            plot_alt.plot(arr_sec,arr_alt,color=PURPLE)
            calcspeed(arr_acc,arr_sec,arr_vel)
            plot_spd.set_val(arr_vel[len(arr_vel)-1])
            plot_vel.clear()
            plot_vel.plot(arr_sec,arr_vel,color=PURPLE)

    except:
        print("Error in Try.")
        fakeGraphs()

try:
    update = animation.FuncAnimation(graphs, animate, interval=200)
    plt.show()
except:
    print("RIP you broke it m8")
