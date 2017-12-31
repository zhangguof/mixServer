#-*- coding:utf-8 -*-
from jinja2 import Environment, FileSystemLoader
import os
import subprocess
from protos.types import *

#dst path
pbs_path = "./pbs" #protobuffer path
cpp_path = "./cpp"
hpp_path = "../../include"

python_path = "../../script/protos"
py_services_path = "../../script/services"
src_path = "./protos"
tempalte_path = "jinja"

proto_bin = "../../bin/protoc"

env = Environment(
    loader=FileSystemLoader(tempalte_path),
    )

def get_pb_type(t):
	types = {
	int:"int32",
	str:"string",
	}
	return types[t]

def gen_pb(mod_name):
	pb_file_path  = os.path.join(pbs_path,mod_name+".proto")
	pb_template = env.get_template("pb_template.proto")
	mod = __import__("protos."+mod_name,globals(),locals(),-1)
	service_id =  mod.SERVICE_ID
	messages = []
	for d in mod.__dict__:
		msg = getattr(mod,d)
		message = {}
		if isinstance(msg,dict) and msg.get("cmd_id"):
			message['name'] = d
			message['service_id'] = service_id
			message['command_id'] = msg["cmd_id"]
			message['args'] = []
			for idx,arg in enumerate(msg['args']):
				t, name = arg
				message['args'].append({
					"required":"required",
					"type":get_pb_type(t),
					"name":name,
					"idx":idx+3,
					})
			messages.append(message)
	s = pb_template.render(messages=messages)
	with open(pb_file_path,"wb") as f:
		f.write(s)
	print "gen pb:%s->%s"%(mod_name,pb_file_path)

def init_mod(mod,mod_name,service_id,file_name):
	mod['name'] = mod_name
	mod['methods'] = []
	mod['service_id'] = service_id
	mod['file_name'] = file_name
	
def gen_cpp_py(_mod_name):
	cpp_server_file_path = os.path.join(cpp_path,"server","gen_"+_mod_name+".cpp")
	cpp_client_file_path = os.path.join(cpp_path,"client","gen_"+_mod_name+".cpp")
	py_file_path = os.path.join(py_services_path,"gen_service.py")
	
	cpp_template = env.get_template("cpp_template.cpp")
	py_template = env.get_template("py_template.py")

	proto_mod = __import__("protos."+_mod_name,globals(),locals(),-1)
	service_id =  proto_mod.SERVICE_ID
	# mods = {} #modname:mod
	file_name = _mod_name
	client_mods = {}
	server_mods = {}
	for d in proto_mod.__dict__:
		msg = getattr(proto_mod,d)
		msg_name = d
		if isinstance(msg,dict) and msg.get("cmd_id"):
			mod_name = msg['mod']
			cmod = client_mods.get(mod_name,{})
			smod = server_mods.get(mod_name,{})
			if len(cmod) == 0:
				init_mod(cmod,mod_name,service_id,file_name)
				client_mods[mod_name] = cmod
			if len(smod) == 0:
				init_mod(smod,mod_name,service_id,file_name)
				server_mods[mod_name] = smod
			method = {"name":msg['func'],
				"command_id":msg['cmd_id'],
				"msg_name":msg_name,
				"side":msg['side'],
				"handle":"py" if msg['handle']==PY else "cpp",
				}
			if msg['side'] & SERVER:
				smod['methods'].append(method)

			if msg['side'] & CLIENT:
				cmod['methods'].append(method)

	s = cpp_template.render(mods=client_mods.values())
	with open(cpp_client_file_path,"wb") as f:
		f.write(s)
	print "gen client cpp:%s->%s"%(_mod_name,cpp_client_file_path)

	s = cpp_template.render(mods=server_mods.values())
	with open(cpp_server_file_path,"wb") as f:
		f.write(s)
	print "gen server cpp:%s->%s"%(_mod_name,cpp_server_file_path)

	s = py_template.render(mods=server_mods.values())
	with open(py_file_path,"wb") as f:
		f.write(s)
	print "gen server py:%s->%s"%(_mod_name,py_file_path)

# bin/protoc --proto_path=proto \
# --cpp_out=proto \
# --python_out=proto \
# proto/test.proto

def gen_pb2cpp(pb_src_path,cpp_dst_path):
	pb_files = []
	for root,_,files in os.walk(pb_src_path):
		for filename in files:
			if filename.endswith(".proto"):
				filepath = os.path.join(root,filename)
				pb_files.append(filepath)
	pbs = " ".join(pb_files)
	args = "--proto_path=%s --cpp_out=%s --python_out=%s %s"% \
			(pb_src_path,cpp_dst_path,python_path,pbs)
	cmd = "%s %s"%(proto_bin,args)
	print subprocess.check_output(cmd, shell=True)
	print "gen cpp from pb:%s-->%s"%(pb_src_path,cpp_dst_path)
	print "gen from files:%s"%(pbs)

def gen_proto_hpp(cpp_pb_path):
	hpp_file_path = os.path.join(hpp_path,"protos.hpp")
	hpp_template = env.get_template("protos_hpp_template.hpp")
	head_files = []
	for _,_,files in os.walk(cpp_pb_path):
		for filename in files:
			if filename.endswith(".pb.h"):
				head_files.append(filename)

		break
	s = hpp_template.render(head_files=head_files)
	with open(hpp_file_path,"wb") as f:
		f.write(s)
	print "gen head file:%s"%hpp_file_path




def gen_files(src_proto_path):
	for _,_,files in os.walk(src_proto_path):
		for filename in files:
			if filename.endswith(".py") and \
			   filename!="__init__.py" and \
			   filename!="types.py":
				mod_name = os.path.splitext(filename)[0]
				gen_pb(mod_name)
				gen_cpp_py(mod_name)
		break


def main():
	gen_files(src_path)
	gen_pb2cpp(pbs_path,cpp_path)
	gen_proto_hpp(cpp_path)


if __name__ == '__main__':
	main()
