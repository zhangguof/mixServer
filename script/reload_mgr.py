#-*- coding:utf-8 -*-
import sys
import weakref
import os
import _engine
disable_reload = True

auto_reload = True

#old mod reloaded object.
#mod_name:[mod_dict1,mod_dict2]
old_mods = {
	
}

type_names  = ("type","classobj")
method_names = ("function", "classmethod", "staticmethod","instancemethod")


def is_type(obj1,obj2):
	return (type(obj1).__name__ in type_names) and\
			(type(obj2).__name__ in type_names)

def is_method(f):
	return type(f).__name__ in method_names

def is_methods(f1,f2):
	return type(f1).__name__ in method_names and \
		   type(f2).__name__ in method_names

def is_dict(o1,o2):
	return type(o1)==dict and type(o2)==dict

#is_mod=True
#in mod dict
def update_dict(old_d,new_d,weak_refs=None,is_mod=False):
	#old_d <= new_d
	if type(old_d) == dict and type(new_d) == dict:
		for k,v in new_d.items():
			if k.startswith("__"):
				continue
			if k in old_d:
				o_val  = old_d[k]
				if is_type(o_val,v):
					#print "update type!,k:%s"%k
					if weak_refs is not None:
						weak_refs[k] = weakref.ref(o_val)
					copy_obj(o_val,v,None)
				elif is_methods(o_val,v):
					old_d[k] = v
					#print "update:%s:old:%s->%s"%(k,o_val,v)
				# elif is_dict(o_val,v):
				# 	if weak_refs is not None:
				# 		weak_refs[k] = weakref.ref(o_val)
				# 	copy_obj(o_val,v)
	else:
		print "update dict:type must dict!"

#ref_dict = {A:ref(A),B:ref(B)}
def copy_dict(old_d,new_d):
	for k, v in old_d.items():
		if k not in new_d:
			continue
		new_v = new_d[k]
		if is_type(v,new_v):
			copy_dict(v.__dict__,new_v.__dict__)
		#elif callable(v) and callable(new_v):
		elif is_methods(v,new_v):
			old_d[k] = new_v

def update_weak_ref(ref_dict,new_d):
	for k, r_v in ref_dict.items():
		assert(type(r_v).__name__ == "weakref")
		if k not in new_d:
			continue
		old_v = r_v()
		if old_v is None:
			del ref_dict[k]
			continue
		new_v = new_d[k]
		assert(is_type(old_v,new_v))
		copy_dict(old_v.__dict__,new_v.__dict__)


#update call able obj.
def copy_obj(old_obj,new_obj,weak_refs,is_mod=False):
	if is_type(old_obj,new_obj):
		copy_obj(old_obj.__dict__,new_obj.__dict__,weak_refs,is_mod)
	elif type(old_obj) == dict and type(new_obj) == dict:
		update_dict(old_obj,new_obj,weak_refs,is_mod)

#mod->mod_dict->
#------------>dicts
#------------>types
#------------------>type.__dict__
def reload_mod(mod_name):
	old_module= sys.modules.get(mod_name)
	if not old_module:
		print "%s mod not import befor!"%mod_name
		return
	old_mod_dict = dict(old_module.__dict__)
	if old_mod_dict.get("disable_reload",False):
		 print "mod:%s,disable_reload!"%mod_name
	if not old_mods.has_key(mod_name):
		old_mods[mod_name] = []
	weak_refs = {}
	
	new_mod = reload(old_module)
	new_mod_dict = new_mod.__dict__
	copy_obj(old_mod_dict,new_mod_dict,weak_refs,True)
	old_refs = old_mods[mod_name]
	for old_ref in old_refs:
		update_weak_ref(old_ref,new_mod_dict)
	#len-1..0
	for idx in xrange(len(old_refs)-1,-1,-1):
		if not old_refs[idx]:
			old_refs.pop(idx)

	old_mods[mod_name].append(weak_refs)
	print "reload mod:%s,success!"%(mod_name)
	# print "=======:"
	# print old_mods
		
#do reload when file modify
file_to_mods = {
	
}
def regist_file_handle(file_path):
	file_path = os.path.abspath(file_path)
	assert(file_path.endswith(".py"))
	cur_path  = os.path.dirname(os.path.abspath(__file__))
	cur_path  = os.path.abspath(cur_path)
	rel_path = os.path.relpath(file_path,cur_path)
	mod_name = rel_path.replace("/",".")[:-3]
	file_to_mods[file_path] = mod_name
	_engine.regist_file_handle(file_path,handle_file_modify);
	print "regist reload mod:%s"%mod_name


def handle_file_modify(file_path):
	mod_name = file_to_mods.get(file_path)
	if not mod_name:
		print "don't regist file:%s"%file_path
	print "going to reload %s"%mod_name
	#do reload
	reload_mod(mod_name)

def get_scripts():
	ex_names = ["reload_mgr.py","__init__.py","init.py","main.py"]
	scripts = []
	for root,_,files in os.walk("script"):
		if root.startswith("script/lib"):
			continue
		if root.startswith("script/google"):
			continue
		for file_name in files:
			if not file_name.endswith(".py"):
				continue
			if file_name in ex_names:
				continue
			file_path = os.path.join(root,file_name)
			scripts.append(file_path)
	return scripts

def init():
	if auto_reload:
		print "auto reload files:"
		files = get_scripts()
		print files
		for file_path in files:
			regist_file_handle(file_path)
		# regist_file_handle("script/test_re.py")





def reload_mods(mod_names):
	pass

def clear_cache():
	old_mods = {}


if __name__ == '__main__':
	pass

