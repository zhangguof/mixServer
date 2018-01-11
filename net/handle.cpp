#include "handle.hpp"
#include "eventloop.hpp"

Handle::Handle()
{
		init(-1,0);
}
Handle::Handle(int _fd,int e)
{
	init(_fd,e);
}
Handle::Handle(int _fd,int e,std::weak_ptr<EventLoop> _loop)
{
	init(_fd,e,_loop);
}
void Handle::init(int _fd,int e,std::weak_ptr<EventLoop> _loop)
{
	init(fd,e);
	ploop = _loop;
}
void Handle::init(int _fd,int e)
{
	// assert(_fd!=-1);
	fd = _fd;
	events = e;
	revents = 0;
	idx = -1;
}

void Handle::handle_event()
{
	// log_debug("handle event!!!:%x",revents);
	if(revents & READ)
		handle_read();
	if(revents & WRITE)
		handle_write();
	if(revents & ERROR)
		handle_error();
	if(revents & MODIFY)
		handle_modify();
}

void Handle::enable_event(int e)
{
	events |= e;
	get_loop()->update_event(get_this());
}
void Handle::disable_event(int e)
{
	events &= (~e);
	get_loop()->update_event(get_this());
}

std::shared_ptr<EventLoop> Handle::get_loop()
{
	auto p = ploop.lock();
	assert(p);
	return p;
}
