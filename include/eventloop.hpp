//Reactor!
//event_loop -> Select|Poll|Epoll?
//

#ifndef _EVENT_LOOP_
#define _EVENT_LOOP_


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "log.hpp"
#include "handle.hpp"
#include "select.hpp"
#include "poll.hpp"

#include "errors.hpp"
#include <map>
#include <set>
#include <memory>
#include <vector>
#include <utility>
#include <cassert>


class EventLoop:public std::enable_shared_from_this<EventLoop>
{
public:
	EventLoop();
	void do_loop();
	void do_select();
	void regist_handle(ptHandle p_handle);
	void unregist_handle(ptHandle p_handle);
	void update_event(ptHandle p_handle);
	void shutdown();
private:
	// Select select_;
	Poll select_;
	std::vector<std::pair<int,int> > active_handles;//fd,events
	std::map<int,ptHandle> handles; //fd:pthandle
	bool _shutdown;

};


#endif