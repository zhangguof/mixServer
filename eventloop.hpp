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
#include <map>
#include <set>
#include <memory>
#include <vector>
#include <cassert>

class Select
{
public:
	static const int maxfds = 1024;
	Select()
	{
		FD_ZERO(&read_fds);
		FD_ZERO(&write_fds);
		FD_ZERO(&error_fds);

		zero_sec.tv_sec  = 0;
		zero_sec.tv_usec = 0;
	}
	void select(std::vector<ptHandle>& active_handles)
	{
		struct fd_set rfds = read_fds;
		struct fd_set wfds = write_fds;
		struct fd_set efds = error_fds;
		// log_debug("is read set:%d",FD_ISSET(all_fds[0],&rfds));

		::select(1024,&rfds,&wfds,&efds,&zero_sec);
		int size = all_fds.size();
		// log_debug("size:%d",size);
		for(int i=0;i<size;++i)
		{
			int fd = all_fds[i];
			auto p_handle = handle_map[fd];
			p_handle->revents = 0;
			if(FD_ISSET(fd,&rfds))
				p_handle->revents |= READ;
			if(FD_ISSET(fd,&wfds))
				p_handle->revents |= WRITE;
			if(FD_ISSET(fd,&efds))
				p_handle->revents |= ERROR;
			if(p_handle->revents)
				active_handles.push_back(p_handle);
		}

	}
	void add_handle(ptHandle p_handle)
	{
		
		int fd = p_handle->get_fd();
		assert(fd!=-1);

		auto find_it = handle_map.find(fd);
		if(find_it == handle_map.end())
		{
			handle_map[fd] = p_handle;
			all_fds.push_back(fd);
			p_handle->idx = all_fds.size()-1;
			
		}
		if(p_handle->events & READ)
			FD_SET(fd,&read_fds);
		if(p_handle->events & WRITE)
			FD_SET(fd,&write_fds);
		if(p_handle->events & ERROR)
			FD_SET(fd,&error_fds);
		log_debug("add_handle:fd:%d,event:%x",
			fd,p_handle->events);
		// log_debug("is read!!%d",FD_ISSET(fd,&read_fds));
	}
	inline void update_fd(int fd,int events,int e,struct fd_set* fds)
	{
		if(!FD_ISSET(fd,fds) && (events&e))
			FD_SET(fd,fds);
		else
			FD_CLR(fd,fds);
	}
	void update_event(ptHandle p_handle)
	{
		int fd = p_handle->get_fd();
		int events = p_handle->events;
		assert(fd!=-1);
		update_fd(fd,events,READ,&read_fds);
		update_fd(fd,events,WRITE,&write_fds);
		update_fd(fd,events,ERROR,&error_fds);
		log_debug("update_handle:fd:%d,event:%x",
			fd,p_handle->events);
	}
	void rm_handle(ptHandle p_handle)
	{
		int fd = p_handle->get_fd();
		auto find_it = handle_map.find(fd);
		log_debug("will rm handle:%d",fd);
		if(find_it != handle_map.end())
		{
			// int idx = p_handle->idx;
			int idx = p_handle->idx;
			if(all_fds.size()>1 && idx!=all_fds.size()-1)
			{
				handle_map[all_fds.back()]->idx = p_handle->idx;
				all_fds[p_handle->idx] = all_fds.back();
			}
			all_fds.pop_back();
			if(p_handle->events & READ)
				FD_CLR(fd,&read_fds);
			if(p_handle->events & WRITE)
				FD_CLR(fd,&write_fds);
			if(p_handle->events & ERROR)
				FD_CLR(fd,&error_fds);
			handle_map.erase(find_it);

			log_debug("rm_handle:fd",fd);
		}
	}
public:
	struct fd_set read_fds;
	struct fd_set write_fds;
	struct fd_set error_fds;
	struct timeval zero_sec;
	std::map<int,ptHandle> handle_map;
	std::vector<int> all_fds;

};


class EventLoop
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
			(*it)->handle_event();
		}
		sleep(2);

	}
	void regist_handle(ptHandle p_handle)
	{
		select_.add_handle(p_handle);
		// handle_map[p_handle->get_fd()] = p_handle;
		// handles.push_back(p_handle);
		// select_.add_fd(p_handle->get_fd(),p_handle->events);
	}
	void unregist_handle(ptHandle p_handle)
	{
		select_.rm_handle(p_handle);
	}
	void update_event(ptHandle p_handle)
	{
		select_.update_event(p_handle);
	}
	void shutdown()
	{
		log_debug("doing loop shutdown...");
		_shutdown = false;
	}
private:

	Select select_;
	std::vector<ptHandle> active_handles;
	bool _shutdown;
	


};


#endif