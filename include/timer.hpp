#ifndef _TIMER_H_
#define _TIMER_H_
#include <memory>
#include <map>
#include <cassert>

typedef unsigned int u32;

class Pri_queue;
class Timer;
class TimerCB
{
public:
	using pttimercb_t = std::shared_ptr<TimerCB>;
	using pttimer_t = std::shared_ptr<Timer>;
	virtual void handle(pttimer_t pt)=0;
	void set_expired(u32 e){expired = e;}
	void set_id(u32 _id)
	{
		timer_id = _id;
	}
	u32 expired;
	u32 timer_id;
	~TimerCB()
	{
		//printf("release timer cb!\n");
	}
};
typedef void (*TimerHandleCBF)(u32,TimerCB::pttimer_t);
class Empty{};

template<typename T=Empty>
class TimerHandle:public TimerCB
{
public:
	typedef std::shared_ptr<TimerHandle<T> > ptTimeHanle;
	typedef std::shared_ptr<T> ptT;
	typedef void (T::*MemFun)(u32,pttimer_t);
	void set_cb(ptT t,MemFun f){
		pObj = t;
		mcb = f;
	}
	void handle(pttimer_t ptimer)
	{
		// if(fcb)
		// 	return fcb(timer_id);
		// assert(pObj);
		if(pObj)
			return (pObj.get()->*mcb)(timer_id,ptimer);

	}
	// static ptTimeHanle make_ptr(ptTimeHanle p,MemFun f)
	// {
	// 	return std::make_shared<TimerHandle<T> >(p,f);
	// }

	TimerHandle(ptT t,MemFun f):pObj(t),mcb(f){}
	// TimerHandle(TimerHandleCBF cb):fcb(cb){}
	TimerHandle():mcb(NULL){}
	
	
	MemFun mcb;
	// TimerHandleCBF fcb;
	ptT pObj;

};
template<>
class TimerHandle<Empty>:public TimerCB
{
public:
	void set_cb(TimerHandleCBF cb)
	{
		fcb = cb;
	}
	void handle(pttimer_t ptimer)
	{
		assert(fcb);
		if(fcb)
			return fcb(timer_id,ptimer);
	}
	u32 timer_id;
	TimerHandleCBF fcb;
};



class Timer:public std::enable_shared_from_this<Timer>
{
public:
	typedef std::shared_ptr<TimerCB> handle_t;
	typedef std::shared_ptr<Timer> pttimer_t;
	typedef u32 time_t;
	Timer();
	u32 start_timer(u32 t,TimerCB::pttimercb_t pHandle);
	void end_timer(TimerCB::pttimercb_t pHandle);
	void update();
	void close();
	inline pttimer_t get_this()
	{
		return shared_from_this();
	}
	template<typename T>
	static typename TimerHandle<T>::ptTimeHanle make_handle(std::shared_ptr<T> pobj,typename TimerHandle<T>::MemFun f);
	static u32 get_now();
	// std::map<int,TimerCB::Shareptr> handles;
	u32 max_id;
	std::shared_ptr<Pri_queue>  pqueue;
};

template<typename T>
typename TimerHandle<T>::ptTimeHanle Timer::make_handle(std::shared_ptr<T> pobj,
	typename TimerHandle<T>::MemFun f)
{
	return std::make_shared<TimerHandle<T> >(pobj,f);
}


#endif