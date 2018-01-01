#include "epoll.hpp"

Epoll::Epoll()
{
	log_debug("EPOL in used!");
	//int epoll_create(int size);
	epoll_events.resize(10);
	n_events = 0; //当前所有的fd数量
	efd = epoll_create(4096);// linux2.6.8后忽略该参数
	if(efd==-1)
		log_debug("epoll_create err!:%d,%s",errno,
			get_error_msg(errno));
	assert(efd!=-1);
}

 // int epoll_wait(int epfd, struct epoll_event *events,
 //                      int maxevents, int timeout);
void Epoll::select(std::vector<std::pair<int,int> >& active_handles )
{
	assert(efd>0);

	int n = ::epoll_wait(efd,&*(epoll_events.begin()),
					   n_events+1,time_out);
	if(n>0)
	{
		for(int i=0;i<n;++i)
		{
			auto epoll_e = epoll_events[i];
			int fd = epoll_e.data.fd;
			int e = from_epoll_event(epoll_e.events);

			active_handles.push_back(std::make_pair(fd,e));
		}
	}
	else if(n<0)
		log_debug("epoll_wait err!!:%d,%s",errno,get_error_msg(errno));
}
//int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
// EPOLL_CTL_ADD
//          Register the target file descriptor fd on the epoll instance
//          referred to by the file descriptor epfd and associate the
//          event event with the internal file linked to fd.

// EPOLL_CTL_MOD
//          Change the event event associated with the target file
//          descriptor fd.

// EPOLL_CTL_DEL
//          Remove (deregister) the target file descriptor fd from the
//          epoll instance referred to by epfd.  The event is ignored and
//          can be NULL (but see BUGS below).
//epoll_event
  // typedef union epoll_data {
  //      void        *ptr;
  //      int          fd;
  //      uint32_t     u32;
  //      uint64_t     u64;
  //  } epoll_data_t;

  //  struct epoll_event {
  //      uint32_t     events;      /* Epoll events */
  //      epoll_data_t data;        /* User data variable */
  //  };
void Epoll::epoll_ctl(int op,int fd,int e)
{
	assert(fd!=-1 && efd>0);
	epoll_event event;
	event.events = to_epoll_event(e); 
	event.data.fd = fd;
	int ret = ::epoll_ctl(efd,op,fd,&event);
	if(ret<0)
		log_debug("epoll_ctl error!:%d:%d:%d,err:%d,%s",fd,e,op,
			 errno,get_error_msg(errno));
	assert(ret!=-1);
}
void Epoll::add_handle(int fd,int e)
{
	log_debug("[epoll]add_handle:%d,%d",fd,e);
	
	epoll_ctl(EPOLL_CTL_ADD,fd,e);
	++n_events;
	if(epoll_events.capacity()<n_events)
		epoll_events.resize(n_events);
}

void Epoll::update_event(int fd, int e)
{
	log_debug("update event:%d,%d",fd,e);
	epoll_ctl(EPOLL_CTL_MOD,fd,e);
}

void Epoll::rm_handle(int fd)
{
	log_debug("rm_handle:%d",fd);
	epoll_ctl(EPOLL_CTL_DEL,fd,0);
	--n_events;
}



#endif
