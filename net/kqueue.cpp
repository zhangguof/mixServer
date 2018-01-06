#include "kqueue.hpp"
#ifdef ENABLE_KQUEUE
#include "eventloop.hpp"

Kqueue::Kqueue()
{
	log_debug("Kqueue in used!");
	log_debug("EVFILT_READ:%0x, EVFILT_WRITE:%0x,EV_ADD:%d,EV_ENABLE:%d,EV_DISABLE:%d,EV_ERROR:%d",
			EVFILT_READ,EVFILT_WRITE,EV_ADD,EV_ENABLE,EV_DISABLE,EV_ERROR);
	log_debug("EACCES:%d,EFAULT:%d,EFAULT:%d,EBADF:%d,EINTR:%d,EINVAL:%d,ENOENT:%d,ENOMEM:%d,ESRCH:%d",
		EACCES,EFAULT,EFAULT,EBADF,EINTR,EINVAL,ENOENT,ENOMEM,ESRCH
		);
	n_events = 0;
	kfd = kqueue();
	k_events.resize(10);
	_timeout.tv_sec = 0;
	_timeout.tv_nsec = timeout*1000*1000;//1ms = 1000us = 10^6 ns;
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
	_active_handles.clear();
	int n = ::kevent(kfd,NULL,0,&*(k_events.begin()),n_events+1,&_timeout); //block
	if(n>0)
	{
		for(int i=0;i<n;++i)
		{

			auto ke = k_events[i];
			int fd = ke.ident;
			int event = from_kqueue_event(ke.filter,ke.flags);
			// log_debug("kqueue:select:fd:%d,event:%d",fd,event);
			//active_handles.push_back(std::make_pair(fd,event));
			auto it = _active_handles.find(fd);
			if(it==_active_handles.end())
				_active_handles[fd] = event;
			else
				it->second |= event;
		}
		for(auto it=_active_handles.begin();it!=_active_handles.end();++it)
		{
			active_handles.push_back(*it);
		}
	}
	else if(n<0)
	{
		log_err("kevent err!:%d,%s",errno,get_error_msg(errno));
		if(errno == EINTR)
		{
			log_debug("EINTR!!,going to shutdown");
			EventLoop::shutdown();
		}
	}
}

void Kqueue::set_kevent(int fd,int e,u16 flags)
{
	// log_debug("set kevent:%d,%0x,%0x",fd,e,flags);
	assert(kfd!=-1 && fd !=-1);
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
		// log_debug("kfd:%d,set_kevent::%d,filter:%d,flags:%d",
			// kfd,event.ident,event.filter,event.flags);
		int n = ::kevent(kfd,&event,1,NULL,0,NULL);
		if(n==-1)
			log_debug("kevent err:%d,%s",errno,get_error_msg(errno));
		assert(n!=-1);
	}

}

void Kqueue::add_handle(int fd,int e)
{
	log_debug("kqueue:add_handle:%d,%d",fd,e);
	
	assert(revents.find(fd) == revents.end());
	// revents[fd] = 0;
	// enable_events[fd] = e;
	set_kevent(fd,e,EV_ADD|EV_ENABLE);
	++n_events;
	revents[fd] = e;
	enable_events[fd] = e;
	if(k_events.capacity()<n_events)
		k_events.resize(n_events);
}
void Kqueue::update_event(int fd,int e)
{
	// log_debug("update event!!!:%d,%d",fd,e);
	auto it = revents.find(fd);
	auto enable_it = enable_events.find(fd);
	assert(it != revents.end());
	assert(enable_it != enable_events.end());

	auto re = it->second; //run events;
	auto enable_e = it->second; //init events;
	u16 flags = 0;
	if(!(enable_e & READ))
		flags = EV_ADD;

	if((e&READ) && !(re&READ))
	{
		set_kevent(fd,READ,flags | EV_ENABLE);
	}
	else if(!(e&READ) && (re&READ))
	{
		set_kevent(fd,READ,flags | EV_DISABLE);
	}

	flags = 0;
	if(!(enable_e & WRITE))
		flags = EV_ADD;
	// log_debug("wirte check!!!,fd:%d,e:%d,re:%d",fd,e,re);
	if((e&WRITE) && !(re&WRITE))
	{
		set_kevent(fd,WRITE,flags | EV_ENABLE);
	}
	else if(!(e&WRITE) && (re&WRITE))
	{
		
		set_kevent(fd,WRITE,flags | EV_DISABLE);
	}
	revents[fd] = e;
	enable_events[fd] |= e;
	log_debug("update event:%d,%0x->%0x",fd,re,e);

}
void Kqueue::rm_handle(int fd)
{
	auto it = revents.find(fd);
	assert(it != revents.end());
	auto re = it->second;
	set_kevent(fd,re,EV_DELETE);
	revents.erase(it);
	enable_events.erase(fd);
	log_debug("rm_handle:%d,%0x",fd,re);
	--n_events;
}

#endif