# Feeds the urw.py fake data from a csv

import csv

data = {'time': -1, 'state': 'no_comm', 'sensors': None}

def giveData():
    with open('flight.py', 'rb') as csvfile:
        dataReader = csv.reader(csvfile)
        for row in dataReader:
            if (isinstance(row, str) == False):
                data['time'] = row[0]
                data['state'] = row[1]
                data['sensors']['acc']['z'] = row[17]
                data['sensors']['bar']['alt'] = row[23]
                #data['sensors']['gps']['lat'] = row[]
                #data['sensors']['gps']['lon'] = row[]

    return data
