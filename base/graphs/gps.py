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
gps_center_x = 0
gps_center_y = 0


## Plots CURRENT gps POINT onto a gps image of the area,
## which can vary based on location
def plot_gps(gps_x,gps_y,gps_x_init,gps_y_init,plot_gps):
    plot_gps.clear()
    
    gps_center_x = gps_x_init
    gps_center_y = gps_y_init
    
    plot_gps.plot(gps_x,gps_y,color=ORANGE)
    plot_gps.set_title(f'GPS [lat:{gps_x},lon:{gps_y}]')
    plot_gps.set_xlabel("Latitude")
    plot_gps.set_ylabel("Longitude")
    #plot_gps.imshow(img,aspect='auto',extent=[min(sec),max(sec),min(gps),max(gps)+500])
    return
