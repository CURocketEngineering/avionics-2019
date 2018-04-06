import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time
import json
import comm

fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)

data = {'time': 0, 'state': 'no_comm', 'sensors': None}
count = 0

accelArray = []
altArray = []

def animate(i):
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
        accelArray.append(states['sensors']['acc']['z'])
        altArray.append(states['sensors']['bar']['alt'])

    accelxar = []
    accelyar = []
    altxar = []
    altyar = []
    for n in dataArray:
        xar.append(int(x))
        yar.append(int(y))
    ax1.clear()
    ax1.plot(xar,yar)

ani = animation.FuncAnimation(fig, animate, interval = 1000)
plt.show()

