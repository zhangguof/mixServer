#ifndef _EPOLL_H_
#define _EPOLL_H_
//only in linux
#ifdef __linux__
#include <sys/epoll.h>
#include <map>
#include <set>
#include <memory>
#include <vector>
#include <utility>
#include <cassert>

#include "log.hpp"
#include "errors.hpp"
#include "handle.hpp"
#include "types.hpp"
class Epoll
{
public:
	static const int time_out = 10;//ms
	Epoll();
	void select(std::vector<std::pair<int,int> >& active_handles);
	void add_handle(int fd,int events);
	void update_event(int fd,int events);
	void rm_handle(int fd);
	void epoll_ctl(int op,int fd,int e);
	inline u32 to_epoll_event(int events)
	{
		u32 e = 0;
		if(events & READ)
			e|=EPOLLIN;
		if(events & WRITE)
			e|=EPOLLOUT;
		return e;
	}

	inline int from_epoll_event(u32 events)
	{
		int e = 0;
		if(events & EPOLLIN)
			e|=READ;
		if(events & EPOLLOUT)
			e|=WRITE;
		if(events & EPOLLERR)
			e|=ERROR;
		return e;
	}
private:
	int efd;
	std::vector<epoll_event> epoll_events;
	int n_events;
	// std::vector<struct pollfd> pollfds;
	// std::map<int,int> handles; //fd:idx

};



#endif
#endif
