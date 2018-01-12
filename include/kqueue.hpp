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
	static const int timeout = 10;//ms
	Kqueue();
	void select(std::vector<std::pair<int,int> >& active_handles);
	void add_handle(int fd,int events);
	void update_event(int fd,int events);
	void rm_handle(int fd);
	void set_kevent(int fd,int e,u16 flags);

	inline int from_kqueue_event(short filter,u16 flags,u32 fflags)
	{
		int e = 0;
		assert(filter == EVFILT_READ || filter == EVFILT_WRITE || filter == EVFILT_VNODE);
		if(filter == EVFILT_READ)
			e|=READ;
		else if(filter == EVFILT_WRITE)
			e|=WRITE;
		else if(filter == EVFILT_VNODE)
		{
			//vnode_events = NOTE_DELETE |  NOTE_WRITE | NOTE_EXTEND | NOTE_ATTRIB | NOTE_LINK | NOTE_RENAME | NOTE_REVOKE;
			if(fflags & NOTE_RENAME || fflags& NOTE_WRITE)
				e|=MODIFY;
			if(fflags & NOTE_DELETE)
				e|=CLOSE;
		}
		if(flags & EV_ERROR)
			e|=ERROR;
		return e;
	}
	inline void _check_and_set_event(int fd,int e,int re,int enable_e,int check_e);


private:
	int kfd;
	std::vector<struct kevent> k_events;
	int n_events;

	// std::vector<struct pollfd> pollfds;
	std::map<int,int> revents; //fd:events
	std::map<int,int> enable_events;//fd:events
	std::map<int,int> _active_handles;
	struct timespec _timeout;
	
};


#endif

#endif