import serial
import time
import threading

class RadioTransreciver:
    """Used for reciving and sending messages to RFM69"""
    #Oprettet 27/09/19 for bachelorprojekt Aarhus Universitet
    #String format:   "NodeID; NetworkID; R/W; Data"


    def __init__(self, NetworkID, verbose = 0):
        """Intialization of RPI RFM69 RadioTransreciver object"""
        #
        self.verbose = verbose;
        self._baudrate = 57600
        self._timeout = 1
        self._NetworkID = NetworkID
        self._CurrentTemp = 0
        self._TargetTemp = 0

        self._BatteryLvl = 0
        self.port = serial.Serial("/dev/ttyUSB0", self._baudrate, timeout=self._timeout)
        time.sleep(0.1)
        if self.verbose:
            print("\n>----------- Settings -----------<")
            print("> NodeID:   {nodeID} : NetworkID: {networkID} <".format(nodeID=000,
                                                                           networkID=self._NetworkID))
            print("> Baud:  {baud} : Timeout:    {timeout} <".format(baud=self._baudrate, timeout=self._timeout))
            print(">>> Initialization Complete! <<<\n")




    def getTemp(self):
        """Asks for the temperature on the current node"""
        print("1. Getting temp")
        self._CurrentTemp = self.Wmsg("RT")
        print("2. Current temp is: {CurrentTemp}\n".format(CurrentTemp = self._CurrentTemp))

    def getValue(self,f_WhatToGet, f_NodeID):
        """
        :param f_WhatToGet: T = tempereature, S = target temp, B = Battery, V = ValvePos
        :param f_NodeID: id of the current node
        :return:
        """

        RW = "R{What}".format(What=f_WhatToGet) # formats what to send
        self.daString = "{NodeID};{RW};!".format(NodeID=f_NodeID, RW=RW)
        print("\n>>Sending :", self.daString)
        self.skriv = self.port.write(self.daString.encode())
        mod = self.port.read(10)
        print("<<Received: {data}\n".format(data=mod.decode()))
        return mod

    def setTarget(self, target):
        """Sets the DRTV's target temperature on the current node"""
        self._TargetTemp = target
        print("Setting target temperature: {temp}".format(temp = self._TargetTemp))
        # self.Wmsg("Set target to {targetTemp}".format(targetTemp = self._TargetTemp), "W")
        self.Wmsg("W44")
        # fik target temp

    def readTarget(self):
        """reads the DRTV's target temperature on the current node"""
        self.currenttarget = self.Wmsg("RS")
        print("Current target temperature: {temp}".format(temp = self.currenttarget))

    def getBatterylvl(self):
        """Asks for the battery level on the current node"""
        print("Getting battery level")
        self._Batterylvl = self.Wmsg("PIK")
        print("Battery lvl is: {bat}".format(bat = self._Batterylvl))

    def Wmsg(self,NodeID, R_W):
        """Stand function for sending message over serial connection"""
        # Make the string: "NodeID; NetworkID; R/W; Data"
        # V1.1 format s  : "NodeID; Messagetype;!"
        self.daString = "{NodeID};{RW};!".format(NodeID = NodeID, RW = R_W)
        # self.daString = "{NodeID}; {NetworkID}; {RW}; {Data}".format(NodeID = self._NodeID, NetworkID = self._NetworkID, Data = Message, RW = R_W)
        if self.verbose:
            print("\n>>Sending :",self.daString)
        self.skriv = self.port.write(self.daString.encode())
        mod = self.port.readline()
        if self.verbose:
            print("<<Received: {data}\n".format(data=mod.decode()))
        return mod.decode()

    def logginLoop(self):
        #threading.Timer(5.0, printit).start()
        print("Hello, World!")



# x = RadioTransreciver(3,2)
# x.Wmsg("test")
# x.getTemp()
# x.setTarget(22)
# x.readTarget()
# x.getBatterylvl()



#x.Read(50)
