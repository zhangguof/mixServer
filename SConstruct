import os
from itertools import chain

src_path = os.path.abspath(os.path.curdir)
print "src_path:",src_path

def add_paths(root_path,paths,exclude_files = []):
	s = set()
	for path in paths:
		for root,dirs,files in os.walk(path):
			for f in files:
				f = os.path.join(root,f)
				if f in exclude_files:
					continue
				if f.endswith(".cpp") or f.endswith(".c") or f.endswith(".cc"):
					s.add(f)
			break
	return list(s)


# env = Environment(TARGET_ARCH=arch_target,CPPDEFINES=[])

# env.Append( CPPPATH=list(chain(include_path,cpp_path)) )
# env.Program(target=target, source=cpp_src, CCFLAGS=cc_flags, LINKFLAGS=link_flags,
# 		LIBS = libs, LIBPATH = lib_path)
google_buffer_src = "../protobuf-3.5.1/src"
google_buffer_libpath = "../protobuf-3.5.1"
google_pb_root = "../protobuf-3.5.1"

python_src= "../Python-2.7.14"
SSL="/usr/local/opt/openssl"

py_include_path = [python_src,python_src+"/Include",SSL+"/include",SSL+"/include/openssl"]
py_lib_path = [python_src,SSL+"/lib"]
py_libs = ["python2.7","ssl","crypto","z"]

proto_path = ["proto","proto/gen_proto/cpp"]

def build_pyext_static(target,root_path,paths,src_files=[],exclude_files=[]):
	srcs = add_paths(root_path,paths,exclude_files)
	srcs.extend(src_files)
	include_path = [root_path+"/python",root_path+"/src"]
	env = Environment(CC = 'c++',CXX='c++',
                   CCFLAGS = '-g -std=c++11')
	env.Append(CPPPATH=list(chain(include_path,paths,
		py_include_path))
	)
	#-lprotobuf -L$(google_buffer_libpath)
	env.StaticLibrary(target, srcs, 
		LIBS=list(chain(["protobuf","python2.7"],py_libs)),
		LIBPATH=list(chain([google_buffer_libpath],py_lib_path)))

# def build_common():
# 	target = "common"


def build(target,paths,exclude_files=[]):
	#paths = [".","net"]
	#target = "cppserver"
	srcs = add_paths(src_path,paths,exclude_files)
	include_path = ["include",google_buffer_src]
	env = Environment(CC = 'c++',CXX='c++',
                   CCFLAGS = '-g -std=c++11 ')
	env.Append(CPPPATH=list(chain(include_path,paths,py_include_path)))
	
	#-lprotobuf -L$(google_buffer_libpath)
	env.Program(target, srcs, 
		LIBS=list(chain(["protobuf","python2.7","pypbext"],py_libs)),
		LIBPATH=list(chain([".",google_buffer_libpath],py_lib_path))
		)

def build_main():
	build_pyext_static("pypbext",
		google_pb_root,
		[google_pb_root+"/python/google/protobuf/pyext",
		 ],
		 [google_pb_root+"/python/google/protobuf/internal/api_implementation.cc",]
		)
	
	build("cppserver",list(
		chain([".","net","services","server","pyengine","proto/gen_proto/cpp/server"],
		proto_path)))
	
	build("client/client",list(
		chain([".","net","client","client/services","pyengine","proto/gen_proto/cpp/client"],
		proto_path)),
	["./main.cpp"])
	
	# build("timer_test",[".","net","test/timer_test"],["./main.cpp"])
	
	

build_main()




