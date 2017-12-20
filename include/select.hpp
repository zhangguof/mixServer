#ifndef _SELECT_H_
#define _SELECT_H_ value

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include <set>
#include <memory>
#include <vector>
#include <utility>
#include <cassert>

#include "log.hpp"
#include "errors.hpp"
#include "handle.hpp"

class Select
{
public:
	static const int maxfds = 65535;
	Select();
	void select(std::vector<std::pair<int,int> >& active_fds);
	void add_handle(int fd,int events);
	inline void update_fd(int fd,int events,int e,struct fd_set* fds);
	void update_event(int fd,int events);
	void rm_handle(int fd);
public:
	struct fd_set read_fds;
	struct fd_set write_fds;
	struct fd_set error_fds;
	struct timeval zero_sec;
	std::vector<int> all_fds;
	std::map<int,int> handles; //fd:idx
	int max_fds;
};

#endif