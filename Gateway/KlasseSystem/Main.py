

import serial
import time
from COM import RadioTransreciver
from RoomModules import Room
import datetime
import threading

import tornado.ioloop
import tornado.web
import tornado.websocket

class MainHandler(tornado.web.RequestHandler):
	def get(self):
	    self.render("index.html")

class SimpleWebSocket(tornado.websocket.WebSocketHandler):
	connections = set()

	def open(self):
	    self.connections.add(self)

	def on_message(self, message):
	    [client.write_message(message) for client in self.connections]

	def on_close(self):
	    self.connections.remove(self)

def make_app():
	return tornado.web.Application([
	    (r"/", MainHandler),
	    (r"/websocket", SimpleWebSocket)
	])

if __name__ == "__main__":
	#Use the Person class to create an object, and then execute the printname method:

	roomObj = Room(4,33,1)
	roomObj.setDescription("Group locale 123")
	aja = 1
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
	app = make_app()
	app.listen(8888)
	

	serverT=threading.Thread(target=tornado.ioloop.IOLoop.instance()).start()
	x = threading.Thread(target=roomObj.pollButtons()).start()
	#y = threading.Thread(target=roomObj.pollButtons())
	#y.start()
	





