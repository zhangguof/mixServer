#-*- coding:utf-*-
import socket

host = ("127.0.0.1",8888)
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect(host)
print s.getsockname(),s.getpeername()