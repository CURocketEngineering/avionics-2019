import math
import sys
import random as random

newfilename = input("File Name (ex. data)\n>> ")
newfilename += ".h"
startheight = float(input("Start Altitude (ex. 100 ft)\n>> "))
par = 0
while par != 'y' and par != 'n':
    par = input("Parachute (y/n)\n>> ")
if par == "y":
    par = False #change true for parachute
else:
    par = False
rocketweight = float(input("Rocket Weight (lbs)\n>> "))
averagethrust = float(input("Average Thrust (N)\n>> "))
thrustduration = float(input("Thrust Duration (s)\n>> "))
bad_zero = int(input("Chance of zero data (0-100)\n>> "))
bad_off = int(input("Chance of wrong data (0-100)\n>> "))

#Writeformat: time, bar_p, bar_alt, *bar_temp, *bar_hum, *gyro_x, *gyro_y, *gyro_z, acc_x, acc_y, acc_z, *mag_x, *mag_y, *mag_z
writefile = open(newfilename, "w")
writefile.write("#ifdef SIM\n\n#include \"sim.h\"\n\nconst struct flight_s flight[] = {\n")

time_prev = 0
bar_alt = startheight
bar_alt_prev = 0
bar_p_prev = 0
acc_x_prev = 0
acc_y_prev = 0
acc_z_prev = 0
halfway = False
vel = (averagethrust * thrustduration)/(7.23 * rocketweight) #estimate in ft/s
print("Start Vel: " + str(vel))

i = 0
while (bar_alt >= startheight or halfway == False):
    time = time_prev + round(random.uniform(9.35,10.65)) #ms
    bar_alt = startheight + ((vel -(32.174*0.01*time))*time*0.01) #does not account for change in pressure and resistance
    halfway = True if (vel -(32.174*0.01*time)) < 0 else False
    bar_p = 0 #idk units so yeah
    acc_x = 0
    acc_y = 0
    acc_z = (vel -(32.174*0.01*time)) / (time*0.01)

    writetime = time
    writealt = bar_alt
    writeacc_z = acc_z
    chance = random.randint(0,100)
    if chance < bad_zero:
        writealt = 0
        writetime = 0
        writeacc_z = 0
    elif chance > bad_zero and chance < bad_zero + bad_off:
        writealt += random.randint(-100000,100000)
        writetime += random.randint(-100000,100000)
        writeacc_z += random.randint(-100000,100000)
    
    writefile.write("\t{ " + str(writetime) + ", "+ str(bar_p) + ", " + str(bar_alt) + ", 0," + " 0," + " 0," + " 0," + " 0, " + str(acc_x) + ", " + str(acc_y) + ", " + str(acc_z) + ", 0," + " 0," + " 0 },\n")

    time_prev = time
    bar_alt_prev = bar_alt
    bar_p_prev = bar_p
    acc_x_prev = acc_x
    acc_y_prev = acc_y
    acc_z_prev = acc_z


writefile.write("};\n\n#endif")
print("\ncompleted")
