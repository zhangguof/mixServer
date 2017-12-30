#-*- coding:utf-8 -*-
#gen by gen_pb.py!!~
import proto
import protos.test_pb2 as test_pb2
import services.Test

def init():
	proto.regist(1000,1,
		test_pb2.Echo,
		services.Test.on_echo)
	proto.regist(1000,1001,
		test_pb2.Add,
		services.Test.on_add)
	pass