## mpl.py - use python3
## reads from quickbox.json

import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.widgets import Slider, Button, RadioButtons
import time
import json
from graphs import barometer as bar

PURPLE = '#522D80'
ORANGE = '#F66733'
state = "Wait"
allJson = []
arr_sec = []
arr_acc = []
arr_alt = []
arr_vel = []
start_time = time.time()

# Animate the graphs
def animate(i):
    try:
        global allJson
        global arr_sec
        global arr_acc
        global arr_alt
        global arr_vel
        global state
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
                plot_acc.grid(True)
                bar.plot_alt(arr_sec,arr_alt,plot_alt)
                plot_vel.clear()
                plot_vel.text(0.5,0.5,state)
                plot_pitch.set_title("pitch")
                plot_yaw.set_title("yaw")
                plot_roll.set_title("roll")
                plot_gps.set_title("gps")
        if 'state' in data:
            state = data['state']
    except:
        print("pass")

try:
    graphs = plt.figure()
    plot_acc = plt.subplot2grid((5,5),(0,4),rowspan=5)
    plot_gps = plt.subplot2grid((5,5),(0,2),colspan=2,rowspan=2)
    plot_alt = plt.subplot2grid((5,5),(2,2),colspan=2,rowspan=2)
    plot_vel = plt.subplot2grid((5,5),(0,0),colspan=2)
    plot_pitch = plt.subplot2grid((5,5),(4,0))
    plot_yaw = plt.subplot2grid((5,5),(4,1))
    plot_roll = plt.subplot2grid((5,5),(4,2))
    plot_tbd = plt.subplot2grid((5,5),(2,0),colspan=2,rowspan=2)

    graphs.patch.set_facecolor(ORANGE)
    graphs.canvas.set_window_title("Avionics Telemetry")
    update = animation.FuncAnimation(graphs, animate)
    #Use subplots_adjust to make graphs look pretty
    plt.tight_layout()
    plt.show()
    
except:
    print("Something went wrong. Contact Charlie Dove at [insert phone number].")
