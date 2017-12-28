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
				if f.endswith(".cpp") or f.endswith(".c") or f.endswith(".cc"):
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
google_buffer_src = "/Users/tony/newwork/github/cpp/protobuf-3.5.1/src"
google_buffer_libpath = "/Users/tony/newwork/github/cpp/protobuf-3.5.1/cmake_build"
def build(target,paths,exclude_files=[]):
	#paths = [".","net"]
	#target = "cppserver"
	srcs = add_paths(src_path,paths,exclude_files)
	include_path = ["include",".","services","proto",google_buffer_src]
	env = Environment(CC = 'c++',CXX='c++',
                   CCFLAGS = '-g -std=c++11')
	env.Append(CPPPATH=list(chain(include_path,paths)))
	#-lprotobuf -L$(google_buffer_libpath)
	env.Program(target, srcs, LIBS=["protobuf"],LIBPATH=[google_buffer_libpath,])

def build_main():
	build("cppserver",[".","net","services","proto"])
	build("client/client",[".","net","client","services","proto"],["./main.cpp"])
	# build("timer_test",[".","net","test/timer_test"],["./main.cpp"])
build_main()




