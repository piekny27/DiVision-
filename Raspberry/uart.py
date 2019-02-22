import RPi.GPIO as GPIO
import serial
import time

ser = serial.Serial("/dev/ttyAMA0")

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.cleanup()

ser.baudrate = 9600

while True:
                var = input("Wprowadz vzad.")
                ser.write(str.encode(var))
                ser.write(str.encode("\n"))
                x=int(ser.read(2),16)
                fromMCP=ser.read(int(x))
                a,b,c=fromMCP.decode().split(',')
                print(a)
                print(b)
                print(c)
ser.close()