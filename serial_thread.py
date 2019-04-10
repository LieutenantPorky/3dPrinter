import time
import serial
import threading

print("Serial init")
ser = serial.Serial('/dev/ttyACM0', timeout = 1)
ser.baudrate = 9600

def serialPrint():
    while True:
        #print("thread run")
        newLine = ser.readline()
        print(newLine)


serThread = threading.Thread(target = serialPrint, daemon=True)
serThread.start()

while True:
    ser.write("<700,700>".encode("utf-8"))
    print("sent message")
    time.sleep(1)
