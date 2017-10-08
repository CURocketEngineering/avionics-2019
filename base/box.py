from __future__ import division, print_function

import json

def init(filename):
    global output

    output = open(filename, 'ab', 0)

    output.write(b'[')

def deinit():
    output.write(b']')
    output.close()

def write(data):
    output.write(json.dumps(data).encode())
    output.write(b',\n')
