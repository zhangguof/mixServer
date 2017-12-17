//Reactor!
//event_loop -> Select|Poll|Epoll?
//

#ifndef _EVENT_LOOP_
#define _EVENT_LOOP_

#include "log.hpp"
#include "channel.hpp"

class EventLoop
{
public:
	EventLoop(){}
	void do_loop()
	{
		while(1)
		{
			;
		}
	}


};


#endif