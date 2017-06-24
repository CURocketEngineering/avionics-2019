from __future__ import division, print_function

def init(filename):
    global output

    output = open(filename, 'ab', 0)

    output.write(b'[')

def deinit():
    output.write(b']')
    output.close()

def write(data):
    output.write(data.encode())
    output.write(b',\n')
