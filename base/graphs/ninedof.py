## ninedof.py

import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.widgets import Slider, Button, RadioButtons
import time
import math

PURPLE = '#522D80'
img = plt.imread("graphs/logo.png")
img[:, :, -1] = 0.5

## Plots Z acceleration as a function of time
def plot_accelerometer():
    return

## Shows animation of current pitch
def plot_pitch(plot_pitch):
    plot_pitch.set_title("Pitch")
    return

## Shows animation of current yaw
def plot_yaw(plot_yaw):
    plot_yaw.set_title("Yaw")
    return

## Shows animation of current roll
def plot_roll(plot_roll):
    plot_roll.set_title("Roll")
    return
