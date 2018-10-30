## mpl.py - use python3
## reads from quickbox.json

import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.widgets import Slider, Button, RadioButtons
import time
import math
import sys
import comm as comm
import serial
import json
from graphs import barometer as bar

PURPLE = '#522D80'
ORANGE = '#F66733'
allJson = []
arr_sec = []
arr_acc = []
arr_alt = []
start_time = time.time()

# Animate the graphs
def animate(i):
    try:
        global allJson
        global arr_sec
        global arr_acc
        global arr_alt
        global start_time
        data = ""
        with open("quickbox.json",'r') as jsonFile:
            data = jsonFile.readlines()
            data = json.loads(data[0])
            print(data)
            if data not in allJson:
                allJson.append(data)
        print("Count: "+str(i)+ " " + str(data))
        if 'sensors' in data:
            if ('time' in data) and ('bar' in data['sensors']):
                arr_acc.append(data['sensors']['acc']['z'])
                arr_alt.append(data['sensors']['bar']['alt'])
                arr_sec.append(time.time()-start_time)
                plot_acc.clear()
                plot_acc.plot(arr_sec,arr_acc,color=PURPLE)
                bar.plot_alt(arr_sec,arr_alt,plot_alt)
                plot_spd.set_val(arr_vel[len(arr_vel)-1])
                plot_vel.clear()
                plot_vel.plot(arr_sec,arr_vel,color=PURPLE)

    except:
        print("pass")


try:
    graphs = plt.figure()
    plot_acc = graphs.add_subplot(2,2,1)
    plot_alt = graphs.add_subplot(2,2,2)
    plot_vel = graphs.add_subplot(2,2,3)
    graphs.patch.set_facecolor(ORANGE)
    graphs.canvas.set_window_title("Avionics Telemetry")
    update = animation.FuncAnimation(graphs, animate, interval=100)
    plt.show()
    
except:
    print("Something went wrong. Contact Charlie Dove at [insert phone number].")
