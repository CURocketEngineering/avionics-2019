def init(filename):
    global output

    output = open(filename, 'ab', 0)

    box.write(b'[')

def deinit():
    output.write(b']')
    output.close()

def write(data):
    output.write(data.encode())
    output.write(b',\n')
