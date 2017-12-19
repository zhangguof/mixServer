#-*- coding:utf-*-
import socket

# print s.getsockname(),s.getpeername()
# s.send("sssss")
def do_recv(s,n):
	return s.recv(n)

def send(s,cmd):
	return s.send(cmd)

def main():
	host = ("127.0.0.1",8889)
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


if __name__ == '__main__':
	main()