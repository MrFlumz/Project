
from COM import RadioTransreciver

class tempModule:
    def __init__(self, f_addr, f_netid):
        self.nodeAdress = f_addr
        self.networkID = f_netid

    def getNodeAddr(self):
        return self.nodeAdress

    def getNetworkID(self):
        return self.networkID

    def getTemp(self):
        radio = RadioTransreciver(self.getNetworkID) # Subclass should use Rooms serial object.
        temp = radio.Wmsg(self.getNodeAddr(), "RT")
        if temp != "":
            return int(temp)
        else:
            print("node {node} unavaiable".format(node=self.getNodeAddr()))
            return -1