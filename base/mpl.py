##Use python3, requires matplotlib
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time
import math
import sys
import comm as comm
from serial import Serial 

##Used for Testing
z = 1
'''
x = [0]
y = [0]
fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)
'''
def fakeAnimate(var):
    global z
    global arr_sec
    global arr_acc
    global arr_alt
    arr_sec.append(z)
    arr_acc.append(math.pow(z,2))
    arr_alt.append(math.sin(z*(3.14/6)))
    plot_acc.clear()
    plot_acc.plot(arr_sec,arr_acc)
    plot_alt.clear()
    plot_alt.plot(arr_sec,arr_alt)
    z += 1
    time.sleep(1)

##Real functions
arr_acc = [0]
arr_alt = [0]
arr_sec = [0]
start_time = time.time()
#Functions for animating altitude and acceleration
def animate(i):
    try:
        global arr_acc
        global arr_alt
        global arr_sec
        message = comm.read()
        if message:
            count = 0
            data['time'] = message['time']
            if message['type'] == 'state':
                data['state'] = message['state']
            elif message['type'] == 'telemetry':
                data['sensors'] = message['sensors']
        else:
            count += 1
            if count > 1000:
                data['state'] = 'no_comm'
                    
        if data['sensors']:
            print("Type: ",str(type(states['sensors']['acc']['z'])))
            arr_acc.append(states['sensors']['acc']['z'])
            arr_alt.append(states['sensors']['bar']['alt'])
            arr_sec.append(time.time()-start_time)
            
            plot_acc.clear()
            plot_acc.plot(arr_sec,arr_acc)
            plot_alt.clear()
            plot_alt.plot(arr_sec,arr_alt)
    except:
        print("Error in Try.")
        fakeGraphs()
    
    
graphs = plt.figure()
graphs.canvas.set_window_title("THE AVIONICS GUYS ARE AWESOME")
plot_acc = graphs.add_subplot(1,2,1)
plot_acc.set_xlabel("time, s")
plot_acc.set_ylabel("g's")
plot_acc.set_title("Acceleration")

plot_alt = graphs.add_subplot(1,2,2)
plot_acc.set_xlabel("time, s")
plot_acc.set_ylabel("ft")
plot_acc.set_title("Altitude")


#update function
def fakeGraphs():
    print("There was an error with comm, graphs will not operate.")
    print("I recommend restarting, unless this is a test")
    update = animation.FuncAnimation(graphs, fakeAnimate,interval=200)
    plt.show()

try:
    update = animation.FuncAnimation(graphs, animate, interval=200)
    plt.show()
except:
    fakeGraphs()
