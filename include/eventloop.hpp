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
// #include "channel.hpp"
#include "handle.hpp"
#include "errors.hpp"
#include <map>
#include <set>
#include <memory>
#include <vector>
#include <utility>
#include <cassert>


class Select
{
public:
	static const int maxfds = 65535;
	Select()
	{
		FD_ZERO(&read_fds);
		FD_ZERO(&write_fds);
		FD_ZERO(&error_fds);

		zero_sec.tv_sec  = 0;
		zero_sec.tv_usec = 0;
		max_fds = 0;
	}
	void select(std::vector<std::pair<int,int> >& active_fds)
	{
		struct fd_set rfds = read_fds;
		struct fd_set wfds = write_fds;
		struct fd_set efds = error_fds;
		// log_debug("is read set:%d",FD_ISSET(all_fds[0],&rfds));
		// log_debug("===max_fds:%d",max_fds);
		int n = ::select(max_fds+1,&rfds,&wfds,&efds,&zero_sec);
		if(n<0)
		{
			log_debug("select error!!!!:%d,%d,%s",n,errno,get_error_msg(errno));
		}
		int size = all_fds.size();
		// log_debug("size:%d",size);
		for(int i=0;i<size;++i)
		{
			int fd = all_fds[i];
			int revents = 0;
			// auto p_handle = handle_map[fd];
			// p_handle->revents = 0;
			if(FD_ISSET(fd,&rfds))
				revents |= READ;
			if(FD_ISSET(fd,&wfds))
				revents |= WRITE;
			if(FD_ISSET(fd,&efds))
				revents |= ERROR;
			if(revents)
				active_fds.push_back(std::make_pair(fd,revents));
		}

	}
	void add_handle(int fd,int events)
	{
		
		// int fd = p_handle->get_fd();
		assert(fd!=-1);
		assert(handles.find(fd) == handles.end());
		if(fd>max_fds)
			max_fds = fd;

		all_fds.push_back(fd);
		handles[fd] = all_fds.size()-1;
		
		if(events & READ)
			FD_SET(fd,&read_fds);
		if(events & WRITE)
			FD_SET(fd,&write_fds);
		if(events & ERROR)
			FD_SET(fd,&error_fds);
		log_debug("add_handle:fd:%d,event:%x",
			fd,events);
		// log_debug("is read!!%d",FD_ISSET(fd,&read_fds));
	}
	inline void update_fd(int fd,int events,int e,struct fd_set* fds)
	{
		int is_inset = FD_ISSET(fd,fds);
		if(is_inset)
		{
			if(!(events&e))
				FD_CLR(fd,fds);
		}
		else if(events&e)
		{
			FD_SET(fd,fds);
		}
	}
	void update_event(int fd,int events)
	{
		// int fd = p_handle->get_fd();
		// int events = p_handle->events;
		assert(fd!=-1);
		update_fd(fd,events,READ,&read_fds);
		update_fd(fd,events,WRITE,&write_fds);
		update_fd(fd,events,ERROR,&error_fds);
		log_debug("update_handle:fd:%d,event:%x",
			fd,events);
	}
	void rm_handle(int fd)
	{
		// int fd = p_handle->get_fd();
		// auto find_it = handle_map.find(fd);
		log_debug("will rm handle:%d",fd);
		auto find_it = handles.find(fd);
		assert(find_it!=handles.end());
		int idx = (*find_it).second;
		if(all_fds.size()>1 && idx!=all_fds.size()-1)
		{
			int last_fd = all_fds.back();
			handles[last_fd] = idx;
			all_fds[idx] = last_fd;
		}
		
		//clear events
		update_fd(fd,0,READ,&read_fds);
		update_fd(fd,0,WRITE,&write_fds);
		update_fd(fd,0,ERROR,&error_fds);

		all_fds.pop_back();
		handles.erase(find_it);
		if(all_fds.empty())
			max_fds = 0;
		else if(fd == max_fds)
			max_fds = (--handles.end())->first;

		log_debug("rm_handle:fd",fd);

	}
public:
	struct fd_set read_fds;
	struct fd_set write_fds;
	struct fd_set error_fds;
	struct timeval zero_sec;
	// std::map<int,ptHandle> handle_map;
	std::vector<int> all_fds;
	std::map<int,int> handles;
	int max_fds;

};


class EventLoop:public std::enable_shared_from_this<EventLoop>
{
public:
	
	EventLoop()
	{
		_shutdown = false;
	}
	void do_loop()
	{
		log_debug("start event loop!");
		while(!_shutdown)
		{
			do_select();
		}
	}
	void do_select()
	{
		active_handles.clear();
		select_.select(active_handles);
		//log_debug("do select!!!size:%d",active_handles.size());
		for(auto it = active_handles.begin();it!=active_handles.end();++it)
		{
			int fd = (*it).first;
			int events = (*it).second;
			// auto p_handle = handles[fd]
			auto it_handle = handles.find(fd);
			assert(it_handle!=handles.end());

			auto p_handle = (*it_handle).second;
			p_handle->revents = events;
			p_handle->handle_event();
			// (*it)->handle_event();
		}
		// sleep(2);

	}
	void regist_handle(ptHandle p_handle)
	{
		int fd = p_handle->get_fd();
		assert(fd!=-1);
		p_handle->set_loop(shared_from_this());
		select_.add_handle(fd,p_handle->events);

		handles[fd] = p_handle;
	}
	void unregist_handle(ptHandle p_handle)
	{
		int fd = p_handle->get_fd();
		select_.rm_handle(p_handle->get_fd());
		auto it = handles.find(fd);
		if(it!=handles.end())
			handles.erase(it);
	}
	void update_event(ptHandle p_handle)
	{
		select_.update_event(p_handle->get_fd(),p_handle->events);
	}
	void shutdown()
	{
		log_debug("doing loop shutdown...");
		_shutdown = false;
	}

private:

	Select select_;
	std::vector<std::pair<int,int> > active_handles;//fd,events
	std::map<int,ptHandle> handles; //fd:pthandle
	bool _shutdown;
	// std::weak_ptr<EventLoop> _this_ptr;
	


};


#endif