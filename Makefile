cpp_src=  server.cpp handles.cpp
include_src =  server.hpp log.hpp Socket.hpp eventloop.hpp handle.hpp
CC=c++
CFLAG=-I. -std=c++11

all: cppserver

cppserver: $(cpp_src) $(include_src)
	$(CC) $(CFLAG) $(cpp_src) -o $(@)

