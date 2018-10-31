## gps.py

import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.widgets import Slider, Button, RadioButtons
import time
import math

ORANGE = '#F66733'
img = plt.imread("graphs/logo.png") # Change to site location
img[:, :, -1] = 0.8
gps_bl = [0,0] #TODO
gps_br = [1,0] #TODO
gps_tl = [0,1] #TODO
gps_tr = [1,1] #TODO

## Plots CURRENT gps POINT onto a gps image of the area,
## which can vary based on location
def plot_gps(gps_x,gps_y,plot_alt):
    plot_alt.clear()
    #map gps
    gps_x = gps_x #TODO
    gps_y = gps_y #TODO
    
    plot_alt.plot(gps_x,gps_y,color=ORANGE)
    plot_alt.set_title("GPS")
    plot_alt.set_xlabel("Latitude")
    plot_alt.set_ylabel("Longitude")
    #plot_alt.imshow(img,aspect='auto',extent=[min(sec),max(sec),min(alt),max(alt)+500])
    return
