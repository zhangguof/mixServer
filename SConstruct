import os
from itertools import chain

src_path = os.path.abspath(os.path.curdir)
print "src_path:",src_path

def add_paths(root_path,paths,exclude_files = []):
	s = set()
	for path in paths:
		# if path == ".":
		# 	path = root_path
		# else:
		# 	path = os.path.join(root_path,path)
		for root,dirs,files in os.walk(path):
			for f in files:
				f = os.path.join(root,f)
				if f in exclude_files:
					continue
				if f.endswith(".cpp") or f.endswith(".c"):
					s.add(f)
			break
	return list(s)

# cppserver_paths = [".","net","include"]
# exclude_files = []
# cppsever_src = add_paths(src_path,cppserver_paths,exclude_files)


# env = Environment(TARGET_ARCH=arch_target,CPPDEFINES=[])

# env.Append( CPPPATH=list(chain(include_path,cpp_path)) )
# env.Program(target=target, source=cpp_src, CCFLAGS=cc_flags, LINKFLAGS=link_flags,
# 		LIBS = libs, LIBPATH = lib_path)

def build(target,paths,exclude_files=[]):
	#paths = [".","net"]
	#target = "cppserver"
	srcs = add_paths(src_path,paths,exclude_files)
	include_path = ["include"]
	env = Environment(CC = 'c++',CXX='c++',
                   CCFLAGS = '-g -std=c++11')
	env.Append(CPPPATH=list(chain(include_path,paths)))
	env.Program(target, srcs, LIBS=[],LIBPATH=[])

def build_main():
	build("cppserver",[".","net"])
	build("client/client",[".","net","client"],["./main.cpp"])
	build("timer_test",[".","net","test/timer_test"],["./main.cpp"])
build_main()




