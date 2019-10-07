

class DRTV:
    def __init__(self, f_addr, f_netid):
        self.nodeAdress = f_addr
        self.networkID = f_netid

    def printinfo(self):
        print(self.nodeAdress, self.networkID)

    def setTemp(self,f_temp):
        print("sending to node "+str(self.nodeAdress))

class Room():
    def __init__(self, f_roomid):
        self.roomID = f_roomid
        self.DRTVList = []

    def addDRTV(self, addr, netid):
        self.DRTVList.append(DRTV(addr,netid))

    def delDRTV(self, addr):
        print("del drtv")

    def setTempAll(self,f_temp):
        for i in self.DRTVList:
            i.setTemp(f_temp)



#Use the Person class to create an object, and then execute the printname method:

roomObj = Room(1)
roomObj.addDRTV(3,33)
roomObj.addDRTV(4,33)
roomObj.setTempAll(54)
