cpp_src= channel.cpp  server.cpp
include_src = channel.hpp server.hpp log.hpp
CC=c++
CFLAG=-I.

all: cppserver

cppserver: $(cpp_src) $(include_src)
	$(CC) $(CFLAG) $(cpp_src) -o $(@)

