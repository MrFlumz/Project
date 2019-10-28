
import serial
import time
from COM import RadioTransreciver
from Module_Button import buttonModule
from Module_Temperature import tempModule
from Module_DTRV import DRTVModule



class Room():
    def __init__(self, f_nodeid, f_netid, f_roomid):
        """Nodeid & NetID is ID of Arduino Gateway"""
        """RoomID is specific for room"""
        self.roomID = f_roomid
        self.NodeID = f_nodeid
        self.NetID = f_netid
        self.DRTVList = []
        self.BUTTONList = []
        self.TempSensorList = []
        self.UserButtonList = []
        self.description = ""
        self.target_temp = 0
        self.time = 0
        self.radio = RadioTransreciver(self.NetID)

    # should be seperate function which checks every room instead
    def pollButtons(self):
        self.temp = 20;
        while True:
            btnstr = self.radio.Wmsg(self.NodeID, "")
            strings = btnstr.split('$')
            print(self.temp)
            print(strings)
            for x in strings:
                str = x.split(';')                              # str indeholder nu $id;+
                length = len(str)
                if str[0].rstrip() != "":                              # tjek at id findes
                    for i in self.BUTTONList:
                        if int(str[0].rstrip()) == i.getNodeAddr():  # tjek at id passer til nuværende rum
                            if str[1].rstrip() != "":
                                if str[1].rstrip() == '+':
                                    self.temp = self.temp + 1;
                                if str[1].rstrip() == '-':
                                    self.temp = self.temp - 1;

                # print("Node {node} pressed {temp}".format(node=str[0], temp=str[1]))
            time.sleep(0.2)

    def setDescription(self, f_des):
        self.description = f_des

    def getTempRoom(self):


        #with open("/home/flum/Desktop/dat/data.txt", "a") as text_file:
        #    print("Node {node} is {temp} deg C".format(node=self.NodeID, temp=(int(self.rtemp) / 10)),file=text_file)
        for i in self.TempSensorList:
            temp = i.getTemp()
            print("Node {node} is {temp} deg C".format(node=i.getNodeAddr(), temp=(int(temp))))
        #with open("/home/flum/Desktop/dat/data.txt", "a") as text_file:
        #    print(self.atemp-(int(self.rtemp)/10),file=text_file)

    ## BUTTON MODULE FUNCTIONS
    def addBUTTON(self, addr, netid):
        self.BUTTONList.append(buttonModule(addr,netid))

    def addTempSensor(self, addr, netid):
        self.TempSensorList.append(tempModule(addr, netid))

    ## DRTV FUNCTIONS
    def addDRTV(self, addr, netid):
        self.DRTVList.append(DRTVModule(addr, netid))

    def delDRTV(self, addr):
        print("del drtv")

    def setTargetAll(self,f_temp):
        for i in self.DRTVList:
            i.setTarget(f_temp)

    def getTempAll(self):
        u = 0;
        for i in self.DRTVList:
            temp = i.getTemp()
            u = u + temp
        for i in self.TempSensorList:
            temp = i.getTemp()
            u = u + temp
        self.atemp = u / 2

    def getTargetAll(self):
        u = 0;
        for i in self.DRTVList:
            i.getTarget()

    def getValveAll(self):
        u = 0;
        for i in self.DRTVList:
            i.getVal()

