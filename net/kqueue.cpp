#include "kqueue.hpp"
#ifdef ENABLE_KQUEUE

Kqueue::Kqueue()
{
	log_debug("Kqueue in used!");
	n_events = 0;
	kfd = kqueue();
	k_events.resize(10);
	assert(kfd!=-1);
}

// int kqueue(void);

// int kevent(int	kq, const struct kevent	*changelist, int nchanges,
// struct	kevent *eventlist, int nevents,
// const struct timespec *timeout);

// EV_SET(kev, ident,	filter,	flags, fflags, data, udata);

 // struct kevent {
 //     uintptr_t ident;	     /*	identifier for this event */
 //     short     filter;	     /*	filter for event */
 //     u_short   flags;	     /*	action flags for kqueue	*/
 //     u_int     fflags;	     /*	filter flag value */
 //     intptr_t  data;	     /*	filter data value */
 //     void      *udata;	     /*	opaque user data identifier */
 // };

//flags:
// EV_ADD	  Adds the event to the	kqueue.	 Re-adding an existing event
// 	  will modify the parameters of	the original event, and	not
// 	  result in a duplicate	entry.	Adding an event	automatically
// 	  enables it, unless overridden	by the EV_DISABLE flag.

// EV_ENABLE	  Permit kevent() to return the	event if it is triggered.

// EV_DISABLE	  Disable the event so kevent()	will not return	it.  The fil-
// 		  ter itself is	not disabled.

// EV_DELETE	  Removes the event from the kqueue.  Events which are
// 	  attached to file descriptors are automatically deleted on
// 	  the last close of the	descriptor.

//filter
// EVFILT_READ
// EVFILT_WRITE

//kevent(kq, NULL, 0, events, MAX_EVENT_COUNT, NULL);

void Kqueue::select(std::vector<std::pair<int,int> >& active_handles)
{
	assert(kfd>0);
	int n = ::kevent(kfd,NULL,0,&*(k_events.begin()),n_events+1,NULL); //block
	if(n>0)
	{
		for(int i=0;i<n;++i)
		{

			auto e = k_events[i];
			int fd = e.ident;
			active_handles.push_back(std::make_pair(
				fd,from_kqueue_event(e.flags)
				));
		}
	}
	else if(n<0)
	{
		log_debug("kevent err!:%d,%s",errno,get_error_msg(errno));
	}
}

void Kqueue::set_kevent(int fd,int e,u16 flags)
{
	struct kevent event;
	if(e & READ)
	{

		EV_SET(&event,fd,EVFILT_READ,flags,0,0,NULL);
		int n = ::kevent(kfd,&event,1,NULL,0,NULL);
		if(n==-1)
			log_debug("kevent err:%d,%s",errno,get_error_msg(errno));
		assert(n!=-1);
	}
	if(e & WRITE)
	{
		EV_SET(&event,fd,EVFILT_WRITE,flags,0,0,NULL);
		int n = ::kevent(kfd,&event,1,NULL,0,NULL);
		if(n==-1)
			log_debug("kevent err:%d,%s",errno,get_error_msg(errno));
		assert(n!=-1);
	}

}

void Kqueue::add_handle(int fd,int e)
{
	set_kevent(fd,e,EV_ADD|EV_ENABLE);
	assert(revents.find(fd) == revents.end());
	revents[fd] = e;
	++n_events;
	if(k_events.capacity()<n_events)
		k_events.resize(n_events);
}
void Kqueue::update_event(int fd,int e)
{
	auto it = revents.find(fd);
	assert(it != revents.end());
	auto re = it->second; //run events;
	if((e&READ) && !(re&READ))
	{
		set_kevent(fd,READ,EV_ENABLE);
	}
	else if(!(e&READ) && (re&READ))
	{
		set_kevent(fd,READ,EV_DISABLE);
	}
	if((e&WRITE) && !(re&WRITE))
	{
		set_kevent(fd,WRITE,EV_ENABLE);
	}
	else if(!(e&WRITE) && !(re&WRITE))
	{
		set_kevent(fd,WRITE,EV_DISABLE);
	}
	revents[fd] = e;
	log_debug("update event:%d,%0x->%0x",re,e);

}
void Kqueue::rm_handle(int fd)
{
	auto it = revents.find(fd);
	assert(it != revents.end());
	auto re = it->second;
	set_kevent(fd,re,EV_DELETE);
	log_debug("rm_handle:%d,%0x",fd,re);
	--n_events;
}

#endif