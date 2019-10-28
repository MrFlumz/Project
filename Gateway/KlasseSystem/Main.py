

import serial
import time
from COM import RadioTransreciver
from RoomModules import Room
import datetime
import threading

#Use the Person class to create an object, and then execute the printname method:

roomObj = Room(4,33,1)
roomObj.setDescription("Group locale 123")

#roomObj.addDRTV(3,33)
#roomObj.addDRTV(10,33)
#a = datetime.datetime.now()
#roomObj.getTempAll()
#roomObj.getValveAll()
#roomObj.getTargetAll()
#roomObj.getTargetAll()
#roomObj.getTempRoom()
#b = datetime.datetime.now()
#print(b-a)

roomObj.addBUTTON(20,33)
#roomObj.addTempSensor(11,33)
roomObj.getTempRoom()


x = threading.Thread(target=roomObj.pollButtons())
#y = threading.Thread(target=roomObj.pollButtons())
#y.start()
x.start()



