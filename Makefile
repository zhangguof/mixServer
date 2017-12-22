src_path= .
cpp_net_src  = $(wildcard $(src_path)/net/*.cpp)
cpp_test_src = $(wildcard $(src_path)/test/*.cpp) $(cpp_net_src) timer.cpp
cpp_server_src = $(wildcard $(src_path)/*.cpp) $(cpp_net_src)
cpp_include_src = $(wildcard $(src_path)/include/*.hpp)

cpp_server_objs= $(addsuffix .o,$(basename $(cpp_server_src)))
cpp_test_objs = $(addsuffix .o,$(basename $(cpp_test_src)))
# head_src = $(wildcard $(src_path/inlcude/*.hpp))

# include_src =  server.hpp log.hpp Socket.hpp eventloop.hpp handle.hpp errors.hpp
CC=c++
LD=ld
LFALG=
CFLAG=-c -g -Iinclude/ -std=c++11

all: cppserver

cppserver: $(cpp_server_objs) $(cpp_include_src)
	$(CC) $(LFALG) $(cpp_server_objs) -o $(@)

test: $(cpp_test_objs) $(cpp_include_src)
	$(CC) $(LFALG) $(cpp_test_objs) -o test_

$(src_path)/%.o: $(src_path)/%.cpp
	$(CC) $(CFLAG) -o $@ $^

$(src_path)/net/%.o: $(src_path)/%.cpp
	$(CC) $(CFLAG) -o $@ $^

$(src_path)/test/%.o: $(src_path)/test/%.cpp
	$(CC) $(CFLAG) -o $@ $^

clean:
	rm $(cpp_test_objs)



