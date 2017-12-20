#-*- coding:utf-*-
import socket
import time

# print s.getsockname(),s.getpeername()
# s.send("sssss")
host = ("127.0.0.1",8889)

def do_recv(s,n):
	return s.recv(n)

def send(s,cmd):
	return s.send(cmd)

def new_s():
	return socket.socket(socket.AF_INET,socket.SOCK_STREAM)

def main():
	s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	s.connect(host)
	print s.getsockname(),s.getpeername()
	while True:
		cmd = raw_input(">>")
		if cmd=="exit":
			break
		if cmd=="r":
			n = raw_input("n?:")
			print "recv:",do_recv(s,int(n))
		else:
			send(s,cmd)

def main2():
	n = 2000
	sockets = []
	cur = time.time()
	for i in xrange(n):
		s = new_s()
		# s.setblocking(0)
		sockets.append(s)
	print "create cost:",time.time()-cur
	cur = time.time()
	for s in sockets:
		s.connect(host)
		#print s.getsockname(),s.getpeername() 
	print "connect all:cost:",time.time()-cur
	time.sleep(3)
	cur = time.time()
	for s in sockets:
		s.close()
	print "close all:cost:",time.time()-cur


if __name__ == '__main__':
	main2()