#-*- coding:utf-8 -*-
from jinja2 import Environment, FileSystemLoader
import os
import subprocess

pbs_path = "./pbs" #protobuffer path
cpp_path = "./cpp"
python_path = "../../script/protos"
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
	
def gen_cpp(mod_name):
	cpp_file_path  = os.path.join(cpp_path,"gen_"+mod_name+".cpp")
	cpp_template = env.get_template("cpp_template.cpp")
	proto_mod = __import__("protos."+mod_name,globals(),locals(),-1)
	service_id =  proto_mod.SERVICE_ID
	mods = {} #modname:mod
	for d in proto_mod.__dict__:
		msg = getattr(proto_mod,d)
		msg_name = d
		if isinstance(msg,dict) and msg.get("cmd_id"):
			mod_name = msg['mod']
			mod = mods.get(mod_name,{})
			if len(mod) == 0:
				mod['name'] = mod_name
				mod['methods'] = []
				mod['service_id'] = service_id
				mods[mod_name] = mod
			mod['methods'].append(
				{"name":msg['func'],
				"command_id":msg['cmd_id'],
				"msg_name":msg_name}
				)


	s = cpp_template.render(mods=mods.values())
	with open(cpp_file_path,"wb") as f:
		f.write(s)
	print "gen cpp:%s->%s"%(mod_name,cpp_file_path)

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



def gen_files(src_proto_path):
	for _,_,files in os.walk(src_proto_path):
		for filename in files:
			if filename.endswith(".py") and filename!="__init__.py":
				mod_name = os.path.splitext(filename)[0]
				gen_pb(mod_name)
				gen_cpp(mod_name)
		break


def main():
	gen_files(src_path)
	gen_pb2cpp(pbs_path,cpp_path)


if __name__ == '__main__':
	main()
