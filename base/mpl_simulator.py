## mpl_simulator.py - python3
import time

dataFile = open("graphs/testData/flight.json","r")
dataFile = dataFile.readlines()
i = 0
dt = 0.01
while(i < len(dataFile)):
    q = open("quickbox.json","w")
    dataFile[i].rstrip("\n")
    q.write(dataFile[i])
    q.close()
    time.sleep(dt)
    i += 1
    
