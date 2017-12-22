src_path= 
cpp_root_src = $(src_path)/timer.cpp
cpp_src= $(wildcard $(src_path)/client/*.cpp $(src_path)/net/*.cpp) $(cpp_root_src)

objs= $(addsuffix .o,$(basename $(cpp_src)))
head_src = $(wildcard $(src_path/inlcude/*.hpp))

# include_src =  server.hpp log.hpp Socket.hpp eventloop.hpp handle.hpp errors.hpp
CC=c++
LD=ld
LFALG=
CFLAG=-c -g -I../include/ -std=c++11

all: client

client: $(objs) $(head_src)
	$(CC) $(LFALG) $(objs) -o $(@)

$(src_path)/%.o: $(src_path)/%.cpp
	$(CC) $(CFLAG) -o $@ $^

$(src_path)/net/%.o: $(src_path)/net/%.cpp
	$(CC) $(CFLAG) -o $@ $^


clean:
	rm $(objs)
	rm client


