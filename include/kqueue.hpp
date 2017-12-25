#ifndef _KQUEUE_H_
#define _KQUEUE_H_
#if (defined(__APPLE__) && defined(__MACH__)) || defined(__FreeBSD__)
#define ENABLE_KQUEUE 1
 #include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>


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

// struct kevent {
// 	     uintptr_t ident;	     /*	identifier for this event */
// 	     short     filter;	     /*	filter for event */
// 	     u_short   flags;	     	action flags for kqueue	
// 	     u_int     fflags;	     /*	filter flag value */
// 	     intptr_t  data;	     /*	filter data value */
// 	     void      *udata;	     /*	opaque user data identifier */
//  };

class Kqueue
{
public:
	static const int time_out = 10;//ms
	Kqueue();
	void select(std::vector<std::pair<int,int> >& active_handles);
	void add_handle(int fd,int events);
	void update_event(int fd,int events);
	void rm_handle(int fd);
	void set_kevent(int fd,int e,u16 flags);
	inline u16 to_kqueue_event(int events)
	{
		u16 e = 0;
		if(events & READ)
			e|=EVFILT_READ;
		if(events & WRITE)
			e|=EVFILT_WRITE;
		return e;
	}

	inline int from_kqueue_event(u16 events)
	{
		int e = 0;
		if(events & EVFILT_READ)
			e|=READ;
		if(events & EVFILT_WRITE)
			e|=WRITE;
		if(events & EV_ERROR)
			e|=ERROR;
		return e;
	}
private:
	int kfd;
	std::vector<struct kevent> k_events;
	int n_events;
	// std::vector<struct pollfd> pollfds;
	std::map<int,int> revents; //fd:events

};




#endif

#endif