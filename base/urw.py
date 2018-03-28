## Loops through updating graphs 

import urwid as urwid
#import term


##
def init():
    #
    pass

##
def update():
    #p
    pass

##
def graph():
    x = [1,2,3,4,5]
    y = [1,10,4,8,5]
    graph = urwid.BarGraph('dark magenta')
    graph.set_data(x,100)
    graph.render()

init()
graph()
while(True):
    pass
    update()
    graph()
