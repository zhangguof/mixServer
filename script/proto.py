#-*- coding:utf-8 -*-
from protos import test_pb2
import _engine

protos = {
	
}

def regist(s_id,c_id,msg_type,handle):
	protos[(s_id,c_id)] = (msg_type,handle)
	# print 'regist:',s_id,c_id,msg_type,handle

def HandleMsg(s_id,c_id,uid,pb_msg_str):
	# print "PY:HandleMsg:",s_id,c_id,uid,[pb_msg_str]
	k = (s_id,c_id)
	if k not in protos:
		print "Warn:can't find proto:%d,%d"%(s_id,c_id)
		return 0
	msg_t, handle = protos[k]
	m = msg_t()
	m.ParseFromString(pb_msg_str)
	handle(uid,m)
	return 1

def send_msg(uid,p_msg):
	s = p_msg.SerializeToString()
	_engine.send(uid,
			p_msg.service_id,
			p_msg.command_id,
			s)	