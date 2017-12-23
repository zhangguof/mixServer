#include "log.hpp"
#include <list>
#include <queue>
#include <memory>
#include <map>
#include <algorithm>

#include <sys/types.h>
#include <cassert>
#include <chrono>
#include <iostream>

#include <unistd.h>
#include "timer.hpp"
#include <cstdlib>


typedef std::chrono::duration<u32,
		std::ratio<1,100> > centisecond;//10ms,1/100 s

typedef std::chrono::steady_clock::time_point time_point;
typedef std::chrono::steady_clock::duration duration;
typedef std::chrono::steady_clock clock_type;


struct TimerCompare
{
	const bool operator()(const TimerCB::pttimercb_t p1, 
		const TimerCB::pttimercb_t p2)
	{
		return p1->expired > p2->expired;
	}
};

class Pri_queue
{
public:
	typedef TimerCB::pttimercb_t T;
	
	void push(const T& t){
		min_heap.push_back(t);
		std::push_heap(min_heap.begin(),min_heap.end(),cmp);
	}
	const T& top()
	{
		assert(!min_heap.empty());
		return min_heap[0];
	}
	void pop()
	{
		std::pop_heap(min_heap.begin(),min_heap.end(),cmp);
		min_heap.pop_back();
	}
	u32 get_min()
	{
		// assert(!min_heap.empty());
		if(min_heap.empty())
			return 0;
		return min_heap[0]->expired;
	}
	void headfiy()
	{

	}
	void remove(const T& t)
	{
		auto find_it = std::find(min_heap.begin(),min_heap.end(),t);
		assert(find_it!=min_heap.end());
		// int idx = find_it - min_heap.begin();
		*find_it = min_heap.back();
		min_heap.pop_back();
		make_heap(find_it,min_heap.end(),cmp);
		// headfiy(int idx,)
	}

	bool empty()
	{
		return min_heap.empty();
	}
	void clear()
	{
		return min_heap.clear();
	}


	TimerCompare cmp;
	std::vector<T> min_heap;
};


Timer::Timer():max_id(0){
	pqueue = std::make_shared<Pri_queue>();
}
u32 Timer::get_now()
{
	clock_type::time_point tp = clock_type::now(); //time point
	return std::chrono::duration_cast<centisecond>(
			tp.time_since_epoch()).count();
}

u32 Timer::_start_timer(u32 t,TimerCB::pttimercb_t pHandle)
{
	++max_id;
	u32 now = get_now();
	pHandle->set_id(max_id);
	pHandle->set_expired(now+t);

	// handles[max_id] = pHandle;
	pqueue->push(pHandle);
	//printf("start timer:%d,t:%d\n",max_id,t);

	return max_id;
}

void Timer::end_timer(TimerCB::pttimercb_t pHandle)
{
	// auto find_it = handles.find(pHandle->timer_id);
	// assert(find_it!=handles.end());
	pqueue->remove(pHandle);
	// handles.erase(find_it);
}
void Timer::update()
{
	if(pqueue->empty())
		return;
	u32 now = get_now();
	u32 min_expired = pqueue->get_min();
	while(!pqueue->empty() && min_expired <= now)
	{
		auto p = pqueue->top();
		p->handle(get_this());
		pqueue->pop();

		min_expired = pqueue->get_min();
	}
}
void Timer::close()
{
	pqueue->clear();
}






