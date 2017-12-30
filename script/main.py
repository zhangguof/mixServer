#-*- coding:utf-8-*- 

import sys
import os
import encodings #init codes?
from protos import test_pb2

def do_copy(src,dst):
	import shutil
	base_path = os.path.dirname(dst)
	# print "doing copy:%s->%s:base:%s"%(src,dst,base_path)
	if not os.path.exists(base_path):
		os.makedirs(base_path)
	if os.path.exists(dst):
		print "exists:%s"%dst
		return
	shutil.copy(src,dst)
	print "copy:%s->%s"%(src,dst)


def search_copy_mods():
	root_="/Users/tony/workspace/github/Python-2.7.14/Lib"
	dst = "./script/lib/python2.7"
	import sys
	mods = sys.modules
	for mod_name,mod in mods.items():
		mod_file = getattr(mod,"__file__",None)
		if mod_file:
			if mod_file.endswith(".pyc"):
				mod_file = mod_file[:-1]
			base_file = os.path.relpath(mod_file,root_)
			if base_file.find("../") >= 0:
				continue
			dst_file = os.path.join(dst,base_file)
			# print base_file, dst_file
			do_copy(mod_file,dst_file)


def get_msg(s):
	# h = hello_pb2.Hello.ParseFromString(s)
	h = test_pb2.Echo()
	h.ParseFromString(s)
	print h

def main():
	print "in main....."
	import service
	service.init()
	# import proto
	# print proto.protos


	# e = test_pb2.Echo()
	# e.msg = "test"
	# s = e.SerializeToString()
	# print [s]
	# get_msg(s)
	# search_copy_mods()



