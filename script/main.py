#-*- coding:utf-8-*- 

import sys
import encodings #init codes?
from protos import test_pb2

def get_msg(s):
	# h = hello_pb2.Hello.ParseFromString(s)
	h = test_pb2.Echo()
	h.ParseFromString(s)
	print h

def main():
	print "in main....."
	
	e = test_pb2.Echo()
	e.msg = "test"
	s = e.SerializeToString()
	print [s]
	get_msg(s)



