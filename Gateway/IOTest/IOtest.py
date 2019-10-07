import serial
import time
import threading

class RadioTransreciver:
    """Used for reciving and sending messages to RFM69"""
    #Oprettet 27/09/19 for bachelorprojekt Aarhus Universitet
    #String format:   "NodeID; NetworkID; R/W; Data"
    
    
    def __init__(self, NodeID, NetworkID):
        """Intialization of RPI RFM69 RadioTransreciver object"""
        #
        self._baudrate = 9600
        self._timeout = 1
        self._NodeID = NodeID
        self._NetworkID = NetworkID
        self._CurrentTemp = 0
        self._TargetTemp = 0
        self._BatteryLvl = 0
        self.port = serial.Serial("/dev/ttyUSB0", self._baudrate, timeout=self._timeout)
        
        print ("\n>----------- Settings -----------<")
        print ("> NodeID:   {nodeID} : NetworkID: {networkID} <".format(nodeID = self._NodeID, networkID = self._NetworkID))
        print ("> Baud:  {baud} : Timeout:    {timeout} <".format(baud = self._baudrate, timeout = self._timeout))
        print(">>> Initialization Complete! <<<\n")
        
    def getTemp(self):
        """Asks for the temperature on the current node"""
        print("1. Getting temp")
        self._CurrentTemp = self.Wmsg("RT")
        print("2. Current temp is: {CurrentTemp}\n".format(CurrentTemp = self._CurrentTemp))
        
    def setTarget(self, target):
        """Sets the DRTV's target temperature on the current node"""
        self._TargetTemp = target
        print("Setting target temperature: {temp}".format(temp = self._TargetTemp))
        #self.Wmsg("Set target to {targetTemp}".format(targetTemp = self._TargetTemp), "W")
        self.Wmsg("W44")
        #fik target temp
        
    def readTarget(self):
        """reads the DRTV's target temperature on the current node"""
        self.currenttarget = self.Wmsg("RS")
        print("Current target temperature: {temp}".format(temp = self.currenttarget))
        
    def getBatterylvl(self):
        """Asks for the battery level on the current node"""
        print("Getting battery level")
        self._Batterylvl = self.Wmsg("PIK")
        print("Battery lvl is: {bat}".format(bat = self._Batterylvl))
        
    def Wmsg(self, R_W):
        """Stand function for sending message over serial connection"""
        #Make the string: "NodeID; NetworkID; R/W; Data"
        #V1.1 format s  : "NodeID; Messagetype;!"
        self.daString = "{NodeID};{RW};!".format(NodeID = self._NodeID, RW = R_W)
        #self.daString = "{NodeID}; {NetworkID}; {RW}; {Data}".format(NodeID = self._NodeID, NetworkID = self._NetworkID, Data = Message, RW = R_W)
        print "\n>>Sending :",self.daString
        self.skriv = self.port.write(self.daString)
        self.mod = self.port.read(100)
        print("<<Received: {data}\n".format(data = self.mod))
        return self.mod
    
    def logginLoop(self):
        threading.Timer(5.0, printit).start()
        print "Hello, World!"
        
        
               
x = RadioTransreciver(3,2)
x.Wmsg("test")
x.getTemp()
x.setTarget(22)
x.readTarget()
x.getBatterylvl()



#x.Read(50)
