import serial
import numpy as np
import matplotlib.pyplot as plt
import threading

print("Serial init")
ser = serial.Serial('/dev/ttyACM0', timeout = 1)
ser.baudrate = 9600

isPlotting = False
stopPlot = False

dataArray = []

def readData():
    global isPlotting
    global stopPlot
    while True:
        current = ser.read_until();
        print(current)
        if isPlotting and len(current) > 2: dataArray.append([float(i) for i in current.decode('utf-8')[:-2].split(',')])
        if current == b"a\r\n": isPlotting = True
        if stopPlot: return None

serThread = threading.Thread(target = readData, daemon=True)
serThread.start()

stop = input()
stopPlot = True

fs = open("outputDat.txt", 'w')
[fs.write(str(i) + '\n') for i in dataArray]
fs.close()

plt.plot([i[0] for i in dataArray], [i[1] for i in dataArray], 'go')
plt.show()
