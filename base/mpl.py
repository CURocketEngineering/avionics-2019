## mpl.py - use python3
## reads from quickbox.json

import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.widgets import Slider, Button, RadioButtons
import time
import json
from graphs import barometer as bar
from graphs import ninedof as nd
from graphs import gps as gps

PURPLE = '#522D80'
ORANGE = '#F66733'
state = "Wait"
allJson = []
arr_sec = []
arr_acc = []
arr_alt = []
arr_vel = []
start_time = time.time()

# Update (animate) the graphs
def animate(i):
    try:
        # TODO - Necessary?
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
            if data not in allJson:
                allJson.append(data)
        #print("Count: "+str(i)+ " " + str(data))
        # If the json has telemetry data, update telemetry
        if 'sensors' in data:
            if ('time' in data) and ('bar' in data['sensors']):
                # Append data
                arr_acc.append(data['sensors']['acc']['z'])
                arr_alt.append(data['sensors']['bar']['alt'])
                #arr_sec.append(time.time()-start_time)
                arr_sec.append(data['time'])
                # Update graphs
                # Everything should be self-contained, like bar.plot_alt()
                bar.plot_acc(arr_sec,arr_acc,plot_acc)
                bar.plot_alt(arr_sec,arr_alt,plot_alt)
                nd.plot_pitch(plot_pitch)
                nd.plot_yaw(plot_yaw)
                nd.plot_roll(plot_roll)
                gps.plot_gps(0.5,0.5,plot_gps)
                plot_vel.clear() # TODO
                plot_vel.text(0.5,0.5,state) # TODO
        if 'state' in data:
            # TODO - we need a better way of knowing current state
            # Thinking of adding extra line to quickbox.json, thoughts?
            state = data['state']
    except:
        print("pass")
        try:
            with open("quickbox.json",'r') as jsonFile:
                data = jsonFile.readlines()
                data = json.loads(data[0])
                print(data)
        except:
            print("pass again")

try:
    graphs = plt.figure() # So we can manipulate things easier
    # Define plots
    plot_alt = plt.subplot2grid((5,5),(0,4),rowspan=5)
    plot_gps = plt.subplot2grid((5,5),(0,2),colspan=2,rowspan=2)
    plot_acc = plt.subplot2grid((5,5),(2,2),colspan=2,rowspan=2)
    plot_vel = plt.subplot2grid((5,5),(0,0),colspan=2)
    plot_pitch = plt.subplot2grid((5,5),(4,0))
    plot_yaw = plt.subplot2grid((5,5),(4,1))
    plot_roll = plt.subplot2grid((5,5),(4,2))
    plot_tbd = plt.subplot2grid((5,5),(2,0),colspan=2,rowspan=2)

    graphs.patch.set_facecolor(ORANGE) # CLEMSON
    graphs.canvas.set_window_title("Avionics Telemetry") # Does not change
    update = animation.FuncAnimation(graphs, animate) # Uses animate function
    # Note: Use subplots_adjust to make graphs look pretty
    # plt.tight_layout() is used to make sure everything fits,
    # looks weird when used dynamically
    plt.tight_layout()
    plt.show() # Show plot
    
except:
    ## Should not happen
    ## If program returns this, YOU messed up
    print("Something went wrong. Contact Charlie Dove at [insert phone number].")
