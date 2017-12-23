#ifndef _TIMER_H_
#define _TIMER_H_
#include <memory>
#include <map>
#include <cassert>

typedef unsigned int u32;

enum CBType
{
	CB_ID_TIMER=0,
	CB_TIMER,
	CB_VOID,
};

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
class Empty{};

template<typename T=Empty>
class TimerHandle:public TimerCB
{
public:
	typedef std::shared_ptr<TimerHandle<T> > ptTimeHanle;
	typedef std::shared_ptr<T> ptT;
	typedef void (T::*MemFun_id_timer)(u32,pttimer_t);
	typedef void (T::*MemFun_timer)(pttimer_t);
	typedef void (T::*MemFun_void)();
	template<typename memf_t>
	void set_cb(ptT t,memf_t f){
		pObj = t;
		// mcb = f;
		_set_cb(t,f);
	}
	void _set_cb(MemFun_id_timer f)
	{
		cb_tyep = CB_ID_TIMER;
		mcb.mcb_id_timer = f;		
	}
	void _set_cb(MemFun_timer f)
	{
		cb_tyep = CB_TIMER;
		mcb.mcb_timer = f;		
	}
	void _set_cb(MemFun_void f)
	{
		cb_tyep = CB_VOID;
		mcb.mcb_void = f;		
	}
	void handle(pttimer_t ptimer)
	{
		assert(pObj);
		switch(cb_tyep)
		{
			case CB_ID_TIMER:
				return ((pObj.get()->*(mcb.mcb_id_timer))) (timer_id,ptimer);
			case CB_TIMER:
				return ((pObj.get())->*(mcb.mcb_timer)) (ptimer);				
			case CB_VOID:
				return ((pObj.get())->*(mcb.mcb_void)) ();
		}
	}
	template<typename memf_t>
	TimerHandle(ptT t,memf_t f):pObj(t){
		_set_cb(f);
	}
	// TimerHandle(TimerHandleCBF cb):fcb(cb){}
	TimerHandle():mcb(NULL){}
	
	union{
		MemFun_id_timer mcb_id_timer;
		MemFun_timer mcb_timer;
		MemFun_void mcb_void;
	} mcb;
	
	// TimerHandleCBF fcb;
	ptT pObj;
	CBType cb_tyep;

};
template<>
class TimerHandle<Empty>:public TimerCB
{
public:
	typedef std::shared_ptr<TimerHandle<> > ptTimeHanle;
	typedef void (*handle_cb_id_timer)(u32,TimerCB::pttimer_t);
	typedef void (*handle_cb_timer)(TimerCB::pttimer_t);
	typedef void (*handle_cb_void)();
	template<typename fun_t>
	TimerHandle(fun_t f){
		set_cb(f);
	}
	// template<typename F>
	void set_cb(handle_cb_id_timer cb)
	{
		fcb.fcb_id_timer = cb;
		cb_tyep = CB_ID_TIMER;
	}
	void set_cb(handle_cb_timer cb)
	{
		fcb.fcb_timer = cb;
		cb_tyep = CB_TIMER;
	}
	void set_cb(handle_cb_void cb)
	{
		fcb.fcb_void = cb;
		cb_tyep = CB_VOID;
	}
	void handle(pttimer_t ptimer)
	{
		assert(fcb.fcb_id_timer);
		switch(cb_tyep)
		{
			case CB_ID_TIMER:
				return fcb.fcb_id_timer(timer_id,ptimer);
			case CB_TIMER:
				return fcb.fcb_timer(ptimer);
			case CB_VOID:
				return fcb.fcb_void();
		}
	}
	union{
		handle_cb_id_timer fcb_id_timer;
		handle_cb_timer fcb_timer;
		handle_cb_void fcb_void;
	} fcb;
	CBType cb_tyep;
};

class Timer:public std::enable_shared_from_this<Timer>
{
public:
	typedef std::shared_ptr<TimerCB> handle_t;
	typedef std::shared_ptr<Timer> pttimer_t;
	typedef u32 time_t;
	Timer();
	// u32 start_timer_by_handle(u32 t,TimerCB::pttimercb_t pHandle);
	u32 _start_timer(u32 t,TimerCB::pttimercb_t pHandle);

	template<typename T,typename memf_t>
	u32 start_timer(u32 t,const std::shared_ptr<T>& pobj,memf_t f);

	template<typename fun_t>
	u32 start_timer(u32 t,fun_t f);

	void end_timer(TimerCB::pttimercb_t pHandle);
	void update();
	void close();
	inline pttimer_t get_this()
	{
		return shared_from_this();
	}
	template<typename T,typename memf_t>
	static typename TimerHandle<T>::ptTimeHanle make_handle(std::shared_ptr<T> pobj,memf_t f);
	
	template<typename fun_t>
	static typename TimerHandle<>::ptTimeHanle make_handle(fun_t f);

	static u32 get_now();
	// std::map<int,TimerCB::Shareptr> handles;
	u32 max_id;
	std::shared_ptr<Pri_queue>  pqueue;
};

template<typename T,typename memf_t>
typename TimerHandle<T>::ptTimeHanle Timer::make_handle(
	std::shared_ptr<T> pobj,
	memf_t f)
{
	return std::make_shared<TimerHandle<T> >(pobj,f);
}

template<typename fun_t>
typename TimerHandle<>::ptTimeHanle Timer::make_handle(fun_t f)
{
	return std::make_shared<TimerHandle<> >(f);
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