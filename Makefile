src_path= .
# cpp_src=  server.cpp handles.cpp errors.cpp 
cpp_src= $(wildcard $(src_path)/*.cpp )
objs= $(addsuffix .o,$(basename $(cpp_src)))
head_src = $(wildcard $(src_path/inlcude/*.hpp))

# include_src =  server.hpp log.hpp Socket.hpp eventloop.hpp handle.hpp errors.hpp
CC=c++
LD=ld
LFALG=
CFLAG=-c -g -Iinclude/ -std=c++11

all: cppserver

cppserver: $(objs) $(head_src)
	$(CC) $(LFALG) $(objs) -o $(@)

$(src_path)/%.o: $(src_path)/%.cpp
	$(CC) $(CFLAG) -o $@ $^

clean:
	rm *.o



