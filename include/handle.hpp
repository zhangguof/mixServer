#ifndef _HANDLE_
#define _HANDLE_
#include <memory>
#include "log.hpp"
#include <cassert>

enum EVENT
{
	READ = 0x01, 
	WRITE = 0x02,
	ERROR = 0x04,
	MODIFY = 0x08, //for file handle
	CLOSE = 0x10,  //for file hanlde close
};
class Handle;
typedef std::shared_ptr<Handle> ptHandle;
class EventLoop;

class Handle:public std::enable_shared_from_this<Handle>
{
public:
	Handle();
	Handle(int _fd,int e);
	Handle(int _fd,int e,std::weak_ptr<EventLoop> _loop);
	void set_loop(std::weak_ptr<EventLoop> _loop)
	{
		ploop = _loop;
	}
	void init(int _fd,int e,std::weak_ptr<EventLoop> _loop);
	void init(int _fd,int e);
	void handle_event();
	virtual void handle_read()=0;
	virtual void handle_write()=0;
	virtual void handle_error()=0;
	virtual void close() = 0;
	virtual void handle_modify(){}

	inline int get_fd()
	{
		return fd;
	}
	void enable_event(int e);
	void disable_event(int e);

	inline void enable_write()
	{
		enable_event(WRITE);
	}
	inline void disable_write()
	{
		disable_event(WRITE);
	}
	inline void enable_read()
	{
		enable_event(READ);
	}
	inline void disable_read()
	{
		disable_event(READ);
	}
	inline void enable_modify()
	{
		enable_event(MODIFY);
	}
	inline void disable_modify()
	{
		disable_event(MODIFY);
	}
	std::shared_ptr<Handle> get_this()
	{
		return shared_from_this();
	}
	std::shared_ptr<EventLoop> get_loop();
	int revents;
	int events;
	int idx;
private:
	int fd;
	std::weak_ptr<EventLoop> ploop;
	


};


#endif