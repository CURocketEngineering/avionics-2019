# Feeds the urw.py fake data from a csv

import csv
import json

data = {'time': -1, 'state': 'no_comm', 'sensors': None}

def giveData(int i):
    with open('flight.csv', 'rb') as csvfile:
        dataReader = csv.reader(csvfile)
        for row in dataReader:
            if (row = i):
                data['time'] = row[0]
                data['state'] = row[1]
                data['sensors']['acc']['z'] = row[17]
                data['sensors']['bar']['alt'] = row[23]
                #data['sensors']['gps']['lat'] = row[]
                #data['sensors']['gps']['lon'] = row[]

    return data
