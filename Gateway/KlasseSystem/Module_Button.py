
class buttonModule:
    def __init__(self, f_addr, f_netid):
        self.nodeAdress = f_addr
        self.networkID = f_netid

    def getNodeAddr(self):
        return self.nodeAdress
