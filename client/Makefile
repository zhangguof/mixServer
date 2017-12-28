src_path= 
cpp_root_src = $(src_path)/timer.cpp
cpp_proto_src = $(wildcard $(src_path)/proto/*.cpp)  $(wildcard $(src_path)/proto/*.cc)
cpp_service_src = $(wildcard $(src_path)/services/*.cpp)

cpp_src= $(wildcard $(src_path)/client/*.cpp $(src_path)/net/*.cpp) $(cpp_root_src) \
		 $(cpp_proto_src) $(cpp_service_src)

objs= $(addsuffix .o,$(basename $(cpp_src)))
head_src = $(wildcard $(src_path/inlcude/*.hpp))

google_buffer_src = /Users/tony/newwork/github/cpp/protobuf-3.5.1/src
google_buffer_libpath = /Users/tony/newwork/github/cpp/protobuf-3.5.1/cmake_build

# include_src =  server.hpp log.hpp Socket.hpp eventloop.hpp handle.hpp errors.hpp
CC=c++
LD=ld
LFALG=-lprotobuf -L$(google_buffer_libpath)
CFLAG=-c -g -I$(src_path)/include -I$(src_path) -I$(google_buffer_src) -I$(src_path)/services  -std=c++11

all: client

client: $(objs) $(head_src)
	$(CC) $(LFALG) $(objs) -o $(@)

$(src_path)/%.o: $(src_path)/%.cpp
	$(CC) $(CFLAG) -o $@ $^

$(src_path)/net/%.o: $(src_path)/net/%.cpp
	$(CC) $(CFLAG) -o $@ $^

$(src_path)/proto/%.o: $(src_path)/proto/%.cpp
	$(CC) $(CFLAG) -o $@ $^

$(src_path)/proto/%.o: $(src_path)/proto/%.cc
	$(CC) $(CFLAG) -o $@ $^

$(src_path)/services/%.o: $(src_path)/services/%.cpp
	$(CC) $(CFLAG) -o $@ $^


clean:
	rm $(objs)
	rm client


