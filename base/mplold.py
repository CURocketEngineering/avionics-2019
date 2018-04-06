#import tkinter as tk
import matplotlib.pyplot as plt
import csv

#Suppress warnings
import warnings
import matplotlib.cbook
warnings.filterwarnings("ignore",category=matplotlib.cbook.mplDeprecation)

Time = [] #s
Stage = []
StageTime = []
MachNumber = [] 
AngleOfAttack = [] #degree
Cd = [] 
Cl = []
Thrust = [] #lbs
Weight = []
Drag = []
Lift = []
CG = []
CP = []
StabilityMargin = []
Acceleration = []
AccelerationV = []
AccelerationH = []
Velocity = [] #ft/s
VelocityV = []
VelicityH = []
PitchAttitude = []
FlightPathAngle = []
Altitude = []
Distance = []
Pressure = []

plt.subplot(2,1,1)
plt.plot(Time, Thrust, label="Thrust Graph")
plt.xlabel("Time, s")
plt.ylabel("Thrust, lbs")
plt.title("Thrust Graph Title")
#plt.legend()

plt.subplot(2,1,2)
plt.plot(Time, MachNumber, label="Mach Number Graph")
plt.xlabel("Time, s")
plt.ylabel("Mach, Number")
plt.title("Mach Graph Title")
#plt.legend()

keepGoing = True
rowCount = 0
maxRows = int(input("Seconds * 100: ")) # 100 = 1 second
with open('samplecsv.csv','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=",")
    rowCount = 0
    for row in plots:
        if (rowCount > 0) and (rowCount < maxRows):
            Time.append(float(row[0]))
            #
            StageTime.append(float(row[2]))
            MachNumber.append(float(row[3]))
            AngleOfAttack.append(float(row[4]))
            #
            #
            Thrust.append(float(row[7]))
            Weight.append(float(row[8]))
            Drag.append(float(row[9]))
            Lift.append(float(row[10]))
            #
            #
            #
            Acceleration.append(float(row[14]))
            #
            #
            Velocity.append(float(row[17]))
            #
            #
            PitchAttitude.append(float(row[20]))
            FlightPathAngle.append(float(row[21]))
            Altitude.append(float(row[22]))
            Distance.append(float(row[23]))
            Pressure.append(float(row[24]))
        rowCount += 1


plt.subplot(2,2,1)
plt.plot(Time, Thrust, label="Thrust Graph")
plt.xlabel("Time, s")
plt.ylabel("Thrust, lbs")
plt.title("Thrust Graph Title")
plt.legend()

plt.subplot(2,2,2)
plt.plot(Time, MachNumber, label="Mach Number Graph")
plt.xlabel("Time, s")
plt.ylabel("Mach, Number")
plt.title("Mach Graph Title")
plt.legend()

plt.subplot(2,2,3)
plt.plot(Time, Velocity, label="Velocity Graph")
plt.xlabel("Time, s")
plt.ylabel("Velocity, ft/s")
plt.title("Velocity Graph Title")
plt.legend()

plt.subplot(2,2,4)
plt.plot(Time, Distance, label="Distance Graph")
plt.xlabel("Time, s")
plt.ylabel("Distance, ft")
plt.title("Distance Graph Title")
plt.legend()

plt.show()

