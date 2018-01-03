#-*- coding:utf-8-*- 

def init():
	print "===in py init!"
	import sys
	sys.path.append("../Python-2.7.14/Lib")
	sys.path.append("../protobuf-3.5.1/python/build/lib.linux-x86_64-2.7")


def test(pyobj):
	print "=========="
	print pyobj
	print type(pyobj)
	print pyobj.num
	print pyobj.test(123,"111")
	print pyobj.foo()
	print "========"
	pyobj.num1  = 123
	pyobj.num2 = 321
	print pyobj.num1,pyobj.num2
	print "========get set test====="
	pyobj.num = 11111111
	print pyobj.num
	pyobj.num = 12345
	print pyobj.num
