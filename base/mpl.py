import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time

fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)
z = 0
global z

def animate(i):
    #pullData = open("sampleText.txt","r").read()
    #dataArray = pullData.split('\n')
    x = []
    y = []
    '''for eachLine in dataArray:
        if len(eachLine)>1:
            x,y = eachLine.split(',')
            xar.append(int(x))
            yar.append(int(y))'''
    ax1.clear()
    ax1.plot(xar,yar)
    z += 1
ani = animation.FuncAnimation(fig, animate, interval=1000)
plt.show()
