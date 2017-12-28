ROOT_DIR = $(shell pwd)
src_path= $(ROOT_DIR)
cpp_net_src  = $(wildcard $(src_path)/net/*.cpp)
cpp_proto_src = $(wildcard $(src_path)/proto/*.cpp)  $(wildcard $(src_path)/proto/*.cc)
cpp_service_src = $(wildcard $(src_path)/services/*.cpp)
cpp_test_src = $(wildcard $(src_path)/test/*.cpp) $(cpp_net_src) timer.cpp
cpp_server_src = $(wildcard $(src_path)/*.cpp) $(cpp_net_src) $(cpp_proto_src) $(cpp_service_src)
cpp_include_src = $(wildcard $(src_path)/include/*.hpp)

cpp_server_objs= $(addsuffix .o,$(basename $(cpp_server_src)))
cpp_test_objs = $(addsuffix .o,$(basename $(cpp_test_src)))
# head_src = $(wildcard $(src_path/inlcude/*.hpp))

google_buffer_src = /Users/tony/newwork/github/cpp/protobuf-3.5.1/src
google_buffer_libpath = /Users/tony/newwork/github/cpp/protobuf-3.5.1/cmake_build

# include_src =  server.hpp log.hpp Socket.hpp eventloop.hpp handle.hpp errors.hpp
CC=c++
LD=ld
LFALG=-lprotobuf -L$(google_buffer_libpath)
CFLAG=-c -g -Iinclude/ -I. -I$(google_buffer_src) -Iservices -std=c++11

all: cppserver client


.PHONY: test timer_test client

test: timer_test

cppserver: $(cpp_server_objs) $(cpp_include_src)
	$(CC) $(LFALG) $(cpp_server_objs) -o $(@)

timer_test:
	make -C $(src_path)/test/$@ src_path=$(src_path)

client:
	echo making client
	make -C $(src_path)/client src_path=$(src_path)


$(src_path)/%.o: $(src_path)/%.cpp
	$(CC) $(CFLAG) -o $@ $^

$(src_path)/net/%.o: $(src_path)/%.cpp
	$(CC) $(CFLAG) -o $@ $^

$(src_path)/proto/%.o: $(src_path)/proto/%.cpp
	$(CC) $(CFLAG) -o $@ $^

$(src_path)/proto/%.o: $(src_path)/proto/%.cc
	$(CC) $(CFLAG) -o $@ $^

$(src_path)/services/%.o: $(src_path)/services/%.cpp
	$(CC) $(CFLAG) -o $@ $^

$(src_path)/test/%.o: $(src_path)/test/%.cpp
	$(CC) $(CFLAG) -o $@ $^

clean:
	rm $(src_path)/*.o
	rm $(src_path)/net/*.o
	rm $(src_path)/client/*.o
	rm $(src_path)/services/*.o
	rm $(src_path)/proto/*.o



