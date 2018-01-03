#-*- coding:utf-8-*- 

def init():
	print "===in py init!"
	import sys
	sys.path.append("../Python-2.7.14/Lib")
	sys.path.append("../protobuf-3.5.1/python/build/lib.linux-x86_64-2.7")

p = {}

def test(pyobj):
	print "=========="
	print pyobj
	print type(pyobj)
	print "num:",pyobj.n
	pyobj.n = 4321
	print "num2:",pyobj.n

	print "========"
	# print pyobj.num
	# pyobj.num = 123
	# print pyobj.num
	
	print pyobj.test(123,"111")
	# print pyobj.foo()
	# print "========"
	# pyobj.num1  = 123
	# pyobj.num2 = 321
	# print pyobj.num1,pyobj.num2
	# print "========get set test====="
	# pyobj.num = 11111111
	# print pyobj.num
	# pyobj.num = 12345
	# print pyobj.num
