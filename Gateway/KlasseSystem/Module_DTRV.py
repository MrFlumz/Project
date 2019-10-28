from COM import RadioTransreciver

class DRTVModule:
    def __init__(self, f_addr, f_netid):
        self.nodeAdress = f_addr
        self.networkID = f_netid

    def getNodeAddr(self):
        return self.nodeAdress

    def getNetworkID(self):
        return self.networkID

    def printinfo(self):
        print(self.nodeAdress, self.networkID)

    def setTarget(self, f_temp):
        radio = RadioTransreciver(self.getNetworkID)
        string = "W" + str(f_temp)
        radio.Wmsg(self.getNodeAddr(), string)

    def getTemp(self):
        radio = RadioTransreciver(self.getNetworkID)
        temp = radio.Wmsg(self.getNodeAddr(), "RT")
        if temp != "":
            with open("/home/flum/Desktop/dat/data.txt", "a") as text_file:
                print("DRTV node {node} is {temp} deg C".format(node=self.getNodeAddr(), temp=(int(temp) / 200)), file=text_file)

            return int(temp) / 200
        else:
            print("node {node} unavaiable".format(node=self.getNodeAddr()))
            return -1

    def getVal(self):
        radio = RadioTransreciver(self.getNetworkID)
        temp = radio.Wmsg(self.getNodeAddr(), "RV")
        if temp != "":
            print("DRTV node {node} valve is {temp} percent open".format(node=self.getNodeAddr(), temp=(int(temp))))
            return
        else:
            print("node {node} unavaiable".format(node=self.getNodeAddr()))

    def getTarget(self):
        radio = RadioTransreciver(self.getNetworkID)
        temp = radio.Wmsg(self.getNodeAddr(),"RS")
        if temp != "":
            print("DRTV node {node} target is {temp} deg C".format(node=self.getNodeAddr(), temp=(int(temp))))
            return
        else:
            print("node {node} unavaiable".format(node=self.getNodeAddr()))

    def getBat(self):
        print("getting Bat from node " + str(self.nodeAdress))

