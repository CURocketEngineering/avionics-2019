## mpl_simulator.py - python3
import time
import json

dataFile = open("graphs/testData/flight.json","r")
dataFile = dataFile.readlines()
i = 0
dt = 0.01
current_state = "wait"
while(i < len(dataFile)):
    q = open("quickbox.json","w")
    dataFile[i].rstrip("\n")
    if 'type' in json.loads(dataFile[i]):
        if (json.loads(dataFile[i])['type'] == "state"):
            current_state = json.loads(dataFile[i])['state']
    q.write(current_state + "\n" + dataFile[i])
    q.close()
    time.sleep(dt)
    i += 1
    
