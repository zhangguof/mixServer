#ifndef _TIMER_H_
#define _TIMER_H_
#include <memory>
#include <map>
#include <cassert>
#include <functional>


typedef unsigned int u32;
class Timer;

typedef std::shared_ptr<Timer> pttimer_t;
typedef std::function<void(std::shared_ptr<Timer>)> timercb_t;
class Pri_queue;

class TimerHandle
{
public:
	typedef std::shared_ptr<TimerHandle> pttimerhandle_t;
	typedef pttimerhandle_t pttimercb_t;
	TimerHandle(u32 t,timercb_t _cb):expired(t),cb(_cb){}
	TimerHandle(timercb_t _cb):cb(_cb){}
	void operator()(pttimer_t  pt)
	{
		cb(pt);
	}
	void set_expired(u32 e){expired = e;}
	void set_id(u32 _id)
	{
		timer_id = _id;
	}
	u32 expired;
	u32 timer_id;
	timercb_t cb;
};

class Timer:public std::enable_shared_from_this<Timer>
{
public:
	typedef std::shared_ptr<TimerHandle> pthandle_t;
	typedef std::shared_ptr<Timer> pttimer_t;
	typedef pthandle_t handle_t;
	typedef u32 time_t;
	Timer();
	u32 _start_timer(u32 t,pthandle_t pHandle);

	template<typename T,typename memf_t>
	u32 start_timer(u32 t,const std::shared_ptr<T>& pobj,memf_t f);

	template<typename fun_t>
	u32 start_timer(u32 t,fun_t f);

	void end_timer(pthandle_t pHandle);
	void update();
	void close();
	inline pttimer_t get_this()
	{
		return shared_from_this();
	}
	template<typename T,typename memf_t>
	static pthandle_t make_handle(std::shared_ptr<T> pobj,memf_t f);
	
	template<typename fun_t>
	static pthandle_t make_handle(fun_t f);

	static u32 get_now();
	u32 max_id;
	std::shared_ptr<Pri_queue>  pqueue;
};

template<typename T,typename memf_t>
Timer::pthandle_t Timer::make_handle(
	std::shared_ptr<T> pobj,
	memf_t f)
{
	timercb_t timer_f = std::bind(f,pobj,std::placeholders::_1);
	return std::make_shared<TimerHandle>(timer_f);


}

template<typename fun_t>
Timer::pthandle_t Timer::make_handle(fun_t f)
{
	timercb_t timer_f = std::bind(f,std::placeholders::_1);
	return std::make_shared<TimerHandle>(timer_f);
}

template<typename T,typename memf_t>
u32 Timer::start_timer(u32 t,const std::shared_ptr<T>& pobj,memf_t f)
{
	return _start_timer(t,make_handle(pobj,f));
}

template<typename fun_t>
u32 Timer::start_timer(u32 t,fun_t f)
{
	return _start_timer(t,make_handle(f));
}



#endif