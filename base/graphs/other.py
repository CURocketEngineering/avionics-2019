## barometer.py

import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.widgets import Slider, Button, RadioButtons
import time
import math

PURPLE = '#522D80'
img = plt.imread("graphs/logo.png")
img[:, :, -1] = 0.5


def plot_data(json_data,state,plot_data):
    plot_data.clear()
    plot_data.set_axis_off()
    plot_data.set_xlim()
    plot_data.set_ylim()
    # set_axis_off, set_xlim, and set_ylim has the effect of removing the axis and just plotting the line
    try:
        plot_data.text(0,0.9,f'State: {state}')
        plot_data.text(0,0.85,f'Time: {json_data["time"]/1000} s')
        plot_data.text(0,0.7,f'Pressure: {json_data["sensors"]["bar"]["p"]}')
        plot_data.text(0,0.5,f'Temperature: {json_data["sensors"]["bar"]["temp"]}')
        plot_data.text(0,0.4,f'Humidity: {state}')
        plot_data.text(0,0,f'Gyro: x {state} y {state} z {state}')
    except:
        pass
    
    return
