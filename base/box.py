# Write to blackbox.json and quickbox.json (for use with mpl)

from __future__ import division, print_function

import json

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
    quickWrite("quickbox.json",data)

def quickWrite(filename,data):
    quick = open(filename,'ab',0)
    quick.write(json.dumbs(data).encode())
    quick.close()
