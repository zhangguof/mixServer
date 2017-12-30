#-*- coding:utf-8 -*-
import proto
from protos import test_pb2
import services
import services.Test
import services.gen_service as gen_service


def init():
	gen_service.init()

