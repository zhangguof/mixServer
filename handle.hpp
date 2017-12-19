#ifndef _HANDLE_
#define _HANDLE_
#include <memory>
#include "log.hpp"

enum EVENT
{
	READ = 0x01, 
	WRITE = 0x02,
	ERROR = 0x04,
};
class Handle;
typedef std::shared_ptr<Handle> ptHandle;
class EventLoop;

class Handle
{
public:
	Handle()
	{
		init(-1,0);
	}
	Handle(int _fd,int e)
	{
		init(_fd,e);
	}
	Handle(int _fd,int e,std::weak_ptr<EventLoop> _loop)
	{
		init(_fd,e,_loop);
	}
	void set_loop(std::weak_ptr<EventLoop> _loop)
	{
		ploop = _loop;
	}
	void init(int _fd,int e,std::weak_ptr<EventLoop> _loop)
	{
		init(fd,e);
		ploop = _loop;

	}
	void init(int _fd,int e)
	{
		fd = _fd;
		events = e;
		revents = 0;
		idx = -1;
	}

	void handle_event()
	{
		// log_debug("handle event!!!:%x",revents);
		if(revents & READ)
			handle_read();
		if(revents & WRITE)
			handle_write();
		if(revents & ERROR)
			handle_error();
	}
	virtual void handle_read()=0;
	virtual void handle_write()=0;
	virtual void handle_error()=0;

	int get_fd()
	{
		return fd;
	}
	void enable_event(int e)
	{
		events |= e;
	}
	void enable_write()
	{
		enable_event(WRITE);
	}
	void disable_event(int e)
	{
		events &= (~e);
	}
	void disable_write()
	{
		disable_event(WRITE);
	}
	int revents;
	int events;
	int idx;
private:
	int fd;
	std::weak_ptr<EventLoop> ploop;
	


};


#endif