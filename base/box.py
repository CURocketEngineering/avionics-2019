# Write to blackbox.json and quickbox.json (for use with mpl)

from __future__ import division, print_function

import json

current_state = "wait"

def init(filename):
    global output
    output = open(filename, 'ab', 0)
    output.write(b'[\n')

def deinit():
    output.write(b']\n')
    output.close()

def write(data):
    output.write(json.dumps(data).encode())
    output.write(b',\n')
    update_state(data)
    quick_write("quickbox.json",data)

def update_state(data):
    try:
        if 'type' in data:
            if data['type'] == "state":
                current_state = data['state']
    except:
        pass

def quick_write(filename,data):
    try:
        quick = open(filename,'ab',0)
        quick.flush()
        quick.write(current_state + "\n" + json.dumps(data).encode())
        quick.close()
    except:
        pass
