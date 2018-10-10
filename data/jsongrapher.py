## jsongrapher.py by Harrison Hall
## graphs height and states for the use of debugging
import json
import matplotlib.pyplot as plt
import math


datafile = open(str(input("JSON to be graphed (ex. flight.json)\n>> ")),"r")
datafile = datafile.readlines()

vert = []
time = []
alt = []
press = []
acc = []

linenum = 0
for line in datafile:
    linenum += 1
    data = json.loads(line)
    if 'state' in data:
        print(str(linenum)+": ",end='')
        print(json.dumps(data))
        vert.append(data["state"])
        vert.append(data["time"])
    if 'sensors' in data:
        if ('time' in data) and ('bar' in data['sensors']):
            time.append(data['time'])
            alt.append([data['sensors']['bar']['alt']])
            press.append([data['sensors']['bar']['p']])
            acc.append([data['sensors']['acc']['z']])

print(vert)

graphs = plt.figure()

clemsonPurple = '#522D80'
clemsonOrange = '#F66733'

graphs.canvas.set_window_title("Sample Data")
plot1 = graphs.add_subplot(2,1,1)
plot1.set_xlabel("time, ms")
plot1ax2 = plot1.twinx()
plot1.set_ylabel("Altitude, ft",color=clemsonPurple)
plot1ax2.set_ylabel("Acceleration, ?units",color=clemsonOrange)
plot1.set_title("Altitude and Z-Acceleration")

plot1.plot(time,alt,color=clemsonPurple)
plot1ax2.plot(time,acc,color=clemsonOrange)

plot2 = graphs.add_subplot(2,1,2)
plot2.set_xlabel("time, ms")
plot2ax2 = plot2.twinx()
plot2.set_ylabel("Pressure, ?units",color=clemsonPurple)
plot2ax2.set_ylabel("Acceleration, ?units",color=clemsonOrange)
plot2.set_title("Pressure and Z-Acceleration")

plot2.plot(time,press,color=clemsonPurple)
plot2ax2.plot(time,acc,color=clemsonOrange)

i = 0
space_alt = 0
space_press = 70000
while(i < len(vert)):
    plot1.axvline(vert[i+1])
    plot1.text(vert[i+1],space_alt,vert[i],color='#86898C')
    plot2.axvline(vert[i+1])
    plot2.text(vert[i+1],space_press,vert[i],color='#86898C')
    i += 2
    space_alt += 2000
    space_press += 5000

plt.show()

#acc bar alt state changes double graph (2 y axis)
