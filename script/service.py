#-*- coding:utf-8 -*-
import proto
from protos import test_pb2
import services
import services.Test


def init():
	proto.regist(1,1,test_pb2.Echo,services.Test.on_echo)


