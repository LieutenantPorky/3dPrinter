#!/usr/bin/env python3

import sys
import serial
import numpy as np
import pynput.mouse as mc
from tkinter import *


#Setup serial connection and debug

'''
print("Serial init")
ser = serial.Serial('/dev/ttyACM0', timeout = 1)
ser.baudrate = 9600
'''
#newLine = ser.read_until()

#define function to handle and send mouse input
#also store position in a string for display
root = Tk()
posString = StringVar()

size = 750

def onMove(x,y):
    #obtain position

    trueX = max(0, min(size, x - (canvas.winfo_x() + root.winfo_x())))
    trueY = max(0, min(size, y - (canvas.winfo_y() + root.winfo_y())))
    print("new pos: " + str(trueX) + ", " + str(trueY))
    posString.set("cursor at {}, {}".format(trueX, trueY))

    #send data to arduino

listener = mc.Listener(on_move=onMove)
listener.start()

# GUI section
#create a class container for the main app loop

class AppWindow:
    frame = None
    widgets = []
    #initialization - place buttons here
    def __init__(self, root):
        self.frame = Frame(root)
        self.frame.pack()

    def addWidgetFxn(self, widgetFxn, kwd, packFxn = None):
        widget = widgetFxn(self.frame, kwd)
        if (packFxn): packFxn(widget)
        else: widget.pack()
        self.widgets.append(widget)

'''
addWidgetFxn takes a functional and optional keywords and pack function to create a widget.
widgetFxn should be a function that returns a widget, for example Button(), and kwd is a
dictionary with keywords for that function.
packFxn is an optional substitute to choose how to pack the widget (and can actually execute
arbitrary code - good hack for init stuff). Should be take the widget as an argument - ex:

lambda widget : widget.pack(side = LEFT)

'''

#Initial setup
mainWindow = AppWindow(root)
#mainWindow.addWidgetFxn(Button,kwd = {"text":"Hello World"}, packFxn = lambda obj : obj.pack(side = LEFT))
# mainWindow.addWidgetFxn(Label,kwd = {"textvariable":"posString"}, packFxn = lambda obj : obj.pack(side = LEFT))

label = Label( root, textvariable=posString, width=20 )
label.pack()

canvas = Canvas(root, cursor="target", height=size, width=size, relief=SUNKEN, bg="white")
lineH = canvas.create_line(0,size/2,size,size/2, dash=(4,2))
lineV = canvas.create_line(size/2,0,size/2,size, dash=(4,2))
canvas.pack()



# Main application loop
root.mainloop()
