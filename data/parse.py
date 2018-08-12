import csv

m = 0.0

with open('flight.csv', 'r') as f1:
    for line in csv.DictReader(f1):
        if float(line['Velocity (ft/sec)']) > m:
            m = float(line['Velocity (ft/sec)'])

print(m)
