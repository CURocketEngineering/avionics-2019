##Use python3, requires matplotlib
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time
import math

##Used for Testing
z = 0
x = [0]
y = [0]
fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)
def fakeAnimate(var):
    #pullData = open("sampleText.txt","r").read()
    #dataArray = pullData.split('\n')
    global z
    x.append(z)
    y.append(math.pow(z,2))
    ax1.clear()
    ax1.plot(x,y)
    z += 1
    time.sleep(1)

##Real functions
#Functions for animating altitude and acceleration
graphs = plt.figure()
plot_acc = graphs.add_subplot(1,2,1)
plot_acc.xlabel("time, s")
plot_acc.ylabel("g's")
plot_acc.title("Acceleration")

plot_alt = graphs.add_subplot(1,2,2)
plot_acc.xlabel("time, s")
plot_acc.ylabel("ft")
plot_acc.title("Altitude")

#update function


update = animation.FuncAnimation(graphs, updateFunction, interval=1000)
graphs.show()
