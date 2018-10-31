## barometer.py

import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.widgets import Slider, Button, RadioButtons
import time
import math

PURPLE = '#522D80'
img = plt.imread("graphs/logo.png")
img[:, :, -1] = 0.5


def plot_alt(arr_sec,arr_alt,plot_alt):
    plot_alt.clear()
    if len(arr_sec) > 50:
        sec = arr_sec[len(arr_sec)-50:]
        alt = arr_alt[len(arr_sec)-50:]
    else:
        sec = arr_sec
        alt = arr_alt
    # set_axis_off, set_xlim, and set_ylim has the effect of removing the axis and just plotting the line
    plot_alt.plot(sec,alt,color=PURPLE)
    plot_alt.set_title("Altitude" + " with " + str(len(sec)) + " length")
    plot_alt.set_xlabel("unit with max "+str(max(sec)))
    plot_alt.set_ylabel("other unit")
    plot_alt.imshow(img,aspect='auto',extent=[min(sec),max(sec),min(alt),max(alt)])
    return
