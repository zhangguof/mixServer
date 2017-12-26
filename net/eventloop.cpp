#include "eventloop.hpp"

EventLoop::EventLoop()
{
	_shutdown = false;
	ptimer = std::make_shared<Timer>();
}
EventLoop::~EventLoop()
{
	log_debug("release eventloop!!");
}

void EventLoop::do_loop()
{
	log_debug("start event loop!");
	while(!_shutdown)
	{
		update();
	}
	clear();

	log_debug("exit event loop!!");
}
void EventLoop::clear()
{
	log_debug("clear event loop....");
	for(auto it=handles.begin();it!=handles.end();++it)
	{
		it->second->close();
	}
	ptimer->close();	
}
void EventLoop::do_select()
{
	active_handles.clear();
	select_.select(active_handles);
	// log_debug("do select!!!size:%d",active_handles.size());
	for(auto it = active_handles.begin();it!=active_handles.end();++it)
	{
		int fd = (*it).first;
		int events = (*it).second;
		log_debug("do select:%d,%d",fd,events);
		auto it_handle = handles.find(fd);
		assert(it_handle!=handles.end());

		auto p_handle = (*it_handle).second;
		p_handle->revents = events;
		p_handle->handle_event();
	}
	// sleep(2);
}

void EventLoop::regist_handle(ptHandle p_handle)
{
	int fd = p_handle->get_fd();
	assert(fd!=-1);
	p_handle->set_loop(shared_from_this());
	select_.add_handle(fd,p_handle->events);

	handles[fd] = p_handle;
}

void EventLoop::unregist_handle(ptHandle p_handle)
{
	int fd = p_handle->get_fd();
	select_.rm_handle(p_handle->get_fd());
	auto it = handles.find(fd);
	if(it!=handles.end())
		handles.erase(it);
	p_handle->revents = 0;//不再处理事件。
}


void EventLoop::update_event(ptHandle p_handle)
{
	select_.update_event(p_handle->get_fd(),p_handle->events);
}

void EventLoop::update()
{
	do_select();
	ptimer->update();
}

void EventLoop::_start_timer(Timer::time_t t,Timer::handle_t ph)
	{
		ptimer->_start_timer(t,ph);
	}

void EventLoop::shutdown()
{
	log_debug("doing loop shutdown...");
	_shutdown = true;
}

