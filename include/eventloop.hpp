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
#include "epoll.hpp"
#include "kqueue.hpp"

#include "errors.hpp"
#include <map>
#include <set>
#include <memory>
#include <vector>
#include <utility>
#include <cassert>
#include "timer.hpp"


class EventLoop:public std::enable_shared_from_this<EventLoop>
{
public:
	typedef std::shared_ptr<EventLoop> ptloop_t;
	EventLoop();
	~EventLoop();
	void do_loop();
	void do_select();
	void regist_handle(ptHandle p_handle);
	void regist_file_handle(const char* fp,ptHandle p_handle);
	void unregist_handle(ptHandle p_handle);
	void update_event(ptHandle p_handle);

	void update();
	void _start_timer(Timer::time_t t,Timer::handle_t ph);

	template<typename T,typename memf_t>
	u32 start_timer(u32 t, const std::shared_ptr<T>& pobj,memf_t f);

	template<typename fun_t>
	u32 start_timer(u32 t,fun_t f);
	
	static void shutdown();
	static bool is_shutdown(){
		return _shutdown;
	}
	void clear();
private:
	// Select select_;
#if defined(ENABLE_EPOLL)
	Epoll select_;
#elif defined(ENABLE_KQUEUE)
	Kqueue select_;
#else
	Poll select_;
#endif
	std::vector<std::pair<int,int> > active_handles;//fd,events
	std::map<int,ptHandle> handles; //fd:pthandle
	Timer::pttimer_t ptimer;
	static bool _shutdown;
	u32 now;

};

template<typename T,typename memf_t>
u32 EventLoop::start_timer(u32 t,const std::shared_ptr<T>& pobj,memf_t f)
{
	return ptimer->start_timer(t,pobj,f);
}

template<typename fun_t>
u32 EventLoop::start_timer(u32 t,fun_t f)
{
	return ptimer->start_timer(t,f);
}


#endif