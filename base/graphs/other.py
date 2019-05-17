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
        newline = "\n"
        plot_data.text(0,0.3,f'State: {state.replace(newline,"")}\nTime: {json_data["time"]/1000} s\nPressure: {json_data["sensors"]["bar"]["p"]}\nTemperature: {json_data["sensors"]["bar"]["temp"]}\nHumidity: {json_data["sensors"]["bar"]["hum"]}\nx {json_data["sensors"]["acc"]["x"]} y {json_data["sensors"]["acc"]["y"]} z {json_data["sensors"]["acc"]["z"]}',verticalalignment='center')
    except:
        pass
    
    return
