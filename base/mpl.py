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
    
    
graphs = plt.figure()
graphs.canvas.set_window_title("THE AVIONICS GUYS ARE AWESOME")
plot_acc = graphs.add_subplot(2,2,1)
plot_acc.set_xlabel("time, s")
plot_acc.set_ylabel("g's")
plot_acc.set_title("Acceleration")

plot_alt = graphs.add_subplot(2,2,2)
plot_alt.set_xlabel("time, s")
plot_alt.set_ylabel("ft")
plot_alt.set_title("Altitude")

plot_vel = graphs.add_subplot(2,2,3)
plot_vel.set_xlabel("time, s")
plot_vel.set_ylabel("ft/s")
plot_vel.set_title("Velocity")

speedometer = plt.axes([0.6, 0.1, 0.3, 0.05], facecolor=PURPLE)
maxspeed = 1050
plot_spd = Slider(speedometer, 'ft/s', 0, maxspeed, valinit=0) 

#cosmetics
plot_acc.set_facecolor((.964,.404,.2))
plot_alt.set_facecolor((.964,.404,.2))
plot_vel.set_facecolor((.964,.404,.2))
graphs.patch.set_facecolor((.52,.53,.55))

arr_acc = [0]
arr_alt = [0]
arr_vel = [0]
arr_sec = [0]
start_time = time.time()

#update function
def fakeGraphs():
    print("There was an error with comm, graphs will not operate.")
    print("I recommend restarting, unless this is a test")
    update = animation.FuncAnimation(graphs, fake_animate,interval=200)
    plt.show()

try:
    update = animation.FuncAnimation(graphs, animate, interval=200)
    plt.show()
except:
    fakeGraphs()
