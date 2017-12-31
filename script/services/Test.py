#-*- coding:utf-8 -*-
from protos import test_pb2
import proto

def on_echo(uid,pb_m):
	msg = pb_m.msg
	print "echo %s"%msg
	n_msg = test_pb2.C_Echo()
	n_msg.msg = "echo :%s"%msg
	proto.send_msg(uid,n_msg)
	

def on_add(uid,pb_m):
	add1 = pb_m.add1
	add2 = pb_m.add2
	print "on add:%d,%d"%(add1,add2)
	
	n_msg = test_pb2.AddRet()
	n_msg.ret = add1 + add2
	proto.send_msg(uid,n_msg)


def on_addret(uid,pb_m):
	ret = pb_m.ret
	print "on addret:%d"%ret