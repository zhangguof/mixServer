#ifndef _EPOLL_H_
#define _EPOLL_H_
//only in linux
#ifdef __linux__
#define ENABLE_EPOLL 1
#include <sys/epoll.h>
#include <sys/inotify.h>  
#include <map>
#include <set>
#include <memory>
#include <vector>
#include <utility>
#include <cassert>
#include <unistd.h>

#include "log.hpp"
#include "errors.hpp"
#include "handle.hpp"
#include "types.hpp"

// struct inotify_event {
//     int      wd;       /* Watch descriptor */
//     uint32_t mask;     /* Mask describing event */
//     uint32_t cookie;    Unique cookie associating related
//                           events (for rename(2)) 
//     uint32_t len;      /* Size of name field */
//     char     name[];   /* Optional null-terminated name */
// };

class iNotifyHandle:public Handle
{
public:
	const static int BUFFER_SIZE = 4096;
	int init(){
		int _fd = ::inotify_init1(IN_NONBLOCK);
		if(_fd==-1)
		{
			log_err("init iNotify error.:%d,%s",
				errno,get_error_msg(errno));
		}
		assert(_fd!=-1);
		Handle::init(_fd,MODIFY);
		has_registed = false;
	}
	void handle_read(){
		log_debug("iNotify handle read!");
		char buf[BUFFER_SIZE];
		int n = ::read(get_fd(),buf,BUFFER_SIZE);
		if(!n)
		{
			log_err("iNotify read 0!!");
			return;
		}
		if(n==-1)
		{
			log_err("iNotify read err!:%s",get_error_msg(errno));
			return;
		}
		for(char* p = buf;p<buf+n;)
		{
			struct inotify_event *event = 
				reinterpret_cast<struct inotify_event*>(p);
			handle_event(event);
			//next p
			p = p + sizeof(struct inotify_event) + event->len;
		}
	}
	void handle_write(){}
	void handle_error(){}
	void close(){}
	void handle_modify(){
		log_debug("handle modify!");
	}
	void handle_event(struct inotify_event* event)
	{
		log_debug("inotify_event:handle event:%d",event->wd);
		if(event->mask & IN_MODIFY)
		{
			auto it = handles.find(event->wd);
			assert(it!=handles.end());
			it->second->handle_modify();
		}
	}
	void add_watch(const char* path,
		std::shared_ptr<Handle> phandle){
		int wd = ::inotify_add_watch(get_fd(),path,IN_MODIFY);
		if(wd==-1)
		{
			log_err("add_watch:err!path:%s,%d,%s",path,
				errno,get_error_msg(errno)
				);
			return;
		}
		assert(wd!=-1);
		watchs[wd] = path;
		// re_watchs[path] = wd;
		handles[wd] = phandle;
	}
	void rm_watch(int wd){
		if(watchs.find(wd)!=watchs.end())
		{
			auto path = watchs[wd];
			watchs.erase(wd);
			// re_watchs.erase(path);
			handles.erase(wd);
			::inotify_rm_watch(get_fd(),wd);
		}
	}
	std::map<int,std::string> watchs;
	// std::map<std::string,int> re_watchs;
	std::map<int,std::shared_ptr<Handle> >handles;
	bool has_registed;
};

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
	std::shared_ptr<iNotifyHandle> pInotify;
private:
	int efd;
	std::vector<epoll_event> epoll_events;
	int n_events;

	// std::vector<struct pollfd> pollfds;
	// std::map<int,int> handles; //fd:idx
};



#endif
#endif
