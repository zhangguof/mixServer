#-*- coding:utf-8 -*-
from protos import test_pb2

protos = {
	
}

def regist(s_id,c_id,msg_type,handle):
	protos[(s_id,c_id)] = (msg_type,handle)
	# print 'regist:',s_id,c_id,msg_type,handle

def HandleMsg(s_id,c_id,uid,pb_msg_str):
	# print "PY:HandleMsg:",s_id,c_id,uid,[pb_msg_str]
	k = (s_id,c_id)
	if k not in protos:
		print "warn:can't find proto:%d,%d"%(s_id,c_id)
		return
	msg_t, handle = protos[k]
	m = msg_t()
	m.ParseFromString(pb_msg_str)
	return handle(uid,m)
	