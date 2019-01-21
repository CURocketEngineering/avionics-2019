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
from graphs import other

PURPLE = '#522D80'
ORANGE = '#F66733'
state = "wait"
allJson = []
arr_sec = []
arr_acc = []
arr_alt = []
arr_vel = []
gps_x_init = 0
gps_y_init = 0
gps_x = 0
gps_y = 0
start_time = time.time()

# Update (animate) the graphs
def animate(i):
    try:
        global allJson, arr_sec, arr_acc, arr_alt, arr_vel
        global gps_x_init, gps_y_init, gps_x, gps_y
        global state, start_time
        data = ""
        with open("quickbox.json",'r') as jsonFile:
            data = jsonFile.readlines()
            state = data[0]
            data = json.loads(data[1])
            if data not in allJson:
                allJson.append(data)
                
        # If the json has telemetry data, update telemetry
        if 'sensors' in data:
            if ('time' in data) and ('bar' in data['sensors']):
                
                # Append data
                arr_acc.append(data['sensors']['acc']['z'])
                arr_alt.append(data['sensors']['bar']['alt'])
                arr_sec.append(data['time'])
                if (gps_x_init == 0 or gps_y_init == 0):
                    gps_x_init = data['sensors']['gps']['lat']
                    gps_y_init = data['sensors']['gps']['lon']
                gps_x = data['sensors']['gps']['lat']
                gps_y = data['sensors']['gps']['lon']
                
                ## Update graphs
                bar.plot_acc(arr_sec,arr_acc,plot_acc)
                bar.plot_alt(arr_sec,arr_alt,plot_alt)
                nd.plot_pitch(plot_pitch)
                nd.plot_yaw(plot_yaw)
                nd.plot_roll(plot_roll)
                gps.plot_gps(gps_x,gps_y,gps_x_init,gps_y_init,plot_gps)
                other.plot_data(data,state,plot_data)
                
    except:
        print("Parse Error")
        try:
            with open("quickbox.json",'r') as jsonFile:
                data = jsonFile.readlines()
                data = json.loads(data[1])
                print(data)
        except:
            print("Parse Correction Error")

try:
    graphs = plt.figure() # So we can manipulate things easier

    # Define plots
    plot_data = plt.subplot2grid((5,5),(0,0),colspan=2,rowspan=2)
    plot_alt = plt.subplot2grid((5,5),(0,4),rowspan=5)
    plot_gps = plt.subplot2grid((5,5),(0,2),colspan=2,rowspan=2)
    plot_acc = plt.subplot2grid((5,5),(2,2),colspan=2,rowspan=2)
    #plot_st = plt.subplot2grid((5,5),(0,0),colspan=2)
    plot_pitch = plt.subplot2grid((5,5),(4,0))
    plot_yaw = plt.subplot2grid((5,5),(4,1))
    plot_roll = plt.subplot2grid((5,5),(4,2))
    plot_tbd = plt.subplot2grid((5,5),(2,0),colspan=2,rowspan=2)

    graphs.patch.set_facecolor(ORANGE)
    graphs.canvas.set_window_title("Avionics Telemetry") 
    update = animation.FuncAnimation(graphs, animate) # Uses animate function
    
    # Note: Use subplots_adjust to make graphs look pretty
    # plt.tight_layout() is used to make sure everything fits,
    # looks weird when used dynamically
    plt.tight_layout()
    plt.show() 
    
except:
    print("Something went wrong. Submit an issue on the github page.")
