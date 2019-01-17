## barometer.py

## TODO - altitude is in feet but HAS to be ABOVE GROUND LEVEL,
## not SEA LEVEL (which I'm assuming it is)

import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.widgets import Slider, Button, RadioButtons
import time
import math

PURPLE = '#522D80'
img = plt.imread("graphs/logo.png")
img[:, :, -1] = 0.5

MAX_HEIGHT = 10000 #ft, varies
height = MAX_HEIGHT * (3/2)

## Plots acceleration in 
## Only plots 50 most recent coordinates
def plot_acc(arr_sec,arr_acc,plot_acc):
    plot_acc.clear()
    if len(arr_sec) > 50:
        sec = arr_sec[len(arr_sec)-50:]
        acc = arr_acc[len(arr_sec)-50:]
    else:
        sec = arr_sec
        acc = arr_acc
        
    # set_axis_off, set_xlim, and set_ylim has the effect
    # of removing the axis and just plotting the line - 
    # for future reference
    plot_acc.plot(sec,acc,color=PURPLE)
    plot_acc.set_title(f'Acceleration [{acc[-1]}]')
    plot_acc.set_ylabel("g's")
    plot_acc.set_xlabel("seconds")
    ''' DEBUG
    plot_alt.set_title("Acceleration" + " with " + str(len(sec)) + " length")
    plot_alt.set_xlabel("unit with max "+str(max(sec)))
    plot_alt.set_ylabel("other unit")
    '''
    plot_acc.imshow(img,aspect='auto',extent=[min(sec),max(sec),min(acc),max(acc)])
    return

## Plots altitude in ft onto a graph
## Plots POINTS of most recent altitude, with scaled coloring
def plot_alt(arr_sec,arr_alt,plot_alt):
    plot_alt.clear()
    plot_alt.plot([0],arr_alt[-1],color=PURPLE,marker='o') # Plot most current point
    plot_alt.axhline(MAX_HEIGHT,-1,1)
    plot_alt.set_title(f'Altitude [{arr_alt[-1]}]')
    plot_alt.set_xlabel("n/a")
    plot_alt.set_ylabel("ft")
    plot_alt.set_ylim([0,height])
    plot_alt.grid(True)
    return

## A speedometer of speed (ft/s)
def plot_speedometer():
    return
