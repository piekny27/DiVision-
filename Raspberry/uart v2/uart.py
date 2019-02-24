import RPi.GPIO as GPIO
import serial
import time

ser = serial.Serial("/dev/ttyAMA0")

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.cleanup()

ser.baudrate = 9600

while True:
                var = input("Wprowadz znak")
                i=0 #zmienna do inkrementacji indeksu segmentów paczki
                ser.write(str.encode(var)) #wysyła rządanie danych do kontrolera
                ser.write(str.encode("\n")) #koniec linii, pomaga kontrolerowi znaleźć koniec wiadomości
                x=int(ser.read(2),16) #czeka na dwa znaki od mikrokontrolera określające długość paczki z danymi, przychodzi w hex - paczka max 256 znaki
                fromMCP=ser.read(int(x)) #odczyt paczki
                fromMCPSplitted=fromMCP.decode().split(',') #podział odczytanej paczki na części
                packCont=fromMCPSplitted[0] #pierwsza część paczki z informacjami z których sensorów przyszły dane
                sensorCount=packCont.count('1') #określa z ilu sensorów przyszły odczyty
                if packCont[0] == '1': 
                    temp=fromMCPSplitted[1]
                    i+=1
                if packCont[1] == '1': 
                    bpm=fromMCPSplitted[1+i]
                    i+=1
                if packCont[2] == '1': 
                    oxidation=fromMCPSplitted[1+i]
                    i+=1
                if==sensorCount: #sprawdzenie na wszelki wypadek czy odczytało dobrze
                    print(temp)
                    print(bpm)
                    print(oxidation)
                    dataFile =open("dataMCP.txt","w") #drugi argument w - napisze ostatnie, a - doda do ostatnich
                    dataFile.write(str(temp))
                    dataFile.write('\n')
                    dataFile.write(str(bpm))
                    dataFile.write('\n')
                    dataFile.write(str(oxidation))
                    dataFile.close()
                elif:
                    print("No cos zle")
ser.close()
