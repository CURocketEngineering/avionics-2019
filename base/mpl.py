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
arr_acc = [0]
arr_alt = [0]
arr_sec = [0]
start_time = time.time()
#Functions for animating altitude and acceleration
def animate(i):
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
        arr_acc.append(states['sensors']['acc']['z'])
        arr_alt.append(states['sensors']['bar']['alt'])
        arr_sec.append(time.time()-start_time)
    
    plot_acc.clear()
    plot_acc.plot(arr_sec,arr_acc)
    plot_alt.clear()
    plot_alt.plot(arr_sec,arr_alt)
    
graphs = plt.figure()
plot_acc = graphs.add_subplot(1,2,1)
plot_acc.set_xlabel("time, s")
plot_acc.set_ylabel("g's")
plot_acc.set_title("Acceleration")

plot_alt = graphs.add_subplot(1,2,2)
plot_acc.set_xlabel("time, s")
plot_acc.set_ylabel("ft")
plot_acc.set_title("Altitude")

#update function


update = animation.FuncAnimation(graphs, animate, interval=200)
graphs.show()

