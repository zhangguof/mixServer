#ifndef _POLL_H_
#define _POLL_H_

#include <poll.h>
#include <map>
#include <set>
#include <memory>
#include <vector>
#include <utility>
#include <cassert>

#include "log.hpp"
#include "errors.hpp"
#include "handle.hpp"

class Poll
{
public:
	static const int time_out = 10; //ms
	Poll();
	void poll(std::vector<std::pair<int,int> >& active_handles);
	void select(std::vector<std::pair<int,int> >& active_handles);
	void add_handle(int fd,int events);
	void update_event(int fd,int events);
	void rm_handle(int fd);
	inline short to_poll_event(int events)
	{
		short e = 0;
		if(events & READ)
			e|=POLLIN;
		if(events & WRITE)
			e|=POLLOUT;
		return e;
	}

	inline int from_poll_event(short events)
	{
		int e = 0;
		if(events & POLLIN)
			e|=READ;
		if(events & POLLOUT)
			e|=WRITE;
		if(events & POLLERR)
			e|=ERROR;
		return e;
	}
private:

	std::vector<struct pollfd> pollfds;
	std::map<int,int> handles; //fd:idx



};

#endif