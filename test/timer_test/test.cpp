#include <map>
#include <iostream>
#include <chrono>
#include <memory>
#include "timer.hpp"
#include "eventloop.hpp"

u32 now()
{
	// clock_type::time_point tp = clock_type::now(); //time point
	// return std::chrono::duration_cast<centisecond>(
	// 		tp.time_since_epoch()).count() * 100;
	return time(NULL);
}

void test_f(Timer::pttimer_t ptimer)
{
	printf("in global func test:%u\n",now());
	ptimer->_start_timer(20,
		Timer::make_handle(test_f));

}

class Test:public std::enable_shared_from_this<Test>
{
public:
	Test()
	{
		printf("Test init!!!!\n");
	}
	typedef std::shared_ptr<Test> ptthis_t;
	// typedef std::shared_ptr<TimerHandle<Test> > PTestHandle;
	void test_f(u32 timer_id,Timer::pttimer_t pt )
	{
		printf("timer cb!!:%u,%u\n",timer_id,now());
		// pt->start_timer(200,Timer::make_handle(get_this(),&Test::test_f));
		pt->start_timer(200,get_this(),&Test::test_f);
	}
	ptthis_t get_this()
	{
		return shared_from_this();
	}
	void test_f2(Timer::pttimer_t pt)
	{
		printf("timer cb2,now:%u\n",now());
		// pt->start_timer(100,Timer::make_handle(get_this(),&Test::test_f2));

	}
	int test;

};

int main()
{
	auto ploop = std::make_shared<EventLoop>();
	auto ptest = std::make_shared<Test>();
	ploop->_start_timer(200,Timer::make_handle(ptest,&Test::test_f));
	// ploop->start_timer(3,Timer::make_handle(ptest,&Test::test_f2));
	// ploop->start_timer(100,Timer::make_handle(test_f));
	ploop->start_timer(300,test_f);
	ploop->do_loop();


	// auto ptest = std::make_shared<Test>();
	// auto pnew = std::make_shared<TimerHandle<Test> >();
	// auto pnew2 = std::make_shared<TimerHandle<> >();
	// pnew->set_cb(ptest,&Test::test_f2);
	// pnew->set_id(1123);

	// pnew2->set_cb(f_cb);
	// pnew2->set_id(1111);

	// typedef std::chrono::duration<double,
	// 		std::ratio<1> > s;//10ms
	// std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
 //    std::cout << "Hello World\n";
 //    usleep(150000); //us,1ms = 1000us
 //    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
 //    std::cout << "Printing took "
 //              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
 //              << "ms.\n";
 //    std::cout << "Printing took "
 //              << std::chrono::duration_cast<centisecond>(end - start).count()
 //              << "*10ms.\n";
 //    std::cout << "Printing took "
 //              << std::chrono::duration_cast<s>(end - start).count()
 //              << "s.\n";

 //    std::chrono::steady_clock::period t;
 //    std::chrono::system_clock::period t1;
 //    std::chrono::high_resolution_clock::period t2;
 //    std::cout<<t.num<<":"<<t.den<<std::endl;
 //    std::cout<<t1.num<<":"<<t1.den<<std::endl;
 //    std::cout<<t2.num<<":"<<t2.den<<std::endl;
 //    std::cout<<std::nano().num<<"/"<<std::nano().den<<std::endl;
	// auto p1 = std::chrono::system_clock::now();
	// std::cout<<(p1.time_since_epoch()).count()<<std::endl;
	// std::cout<< std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count()<<"s"<<std::endl;
	// std::cout<<"test:"<<Timer().get_now()<<std::endl;



	// Timer timer;
	// auto ptest = std::make_shared<Test>();
	// auto p = std::make_shared<TimerHandle<Test> >();
	// p->set_cb(ptest,&Test::test_f);
	// timer.start_timer(200,p);//2s
	// timer.start_timer(1000,TimerHandle<Test>::make_ptr(ptest,&Test::test_f2));
	// auto cur = now();
	// for(int i=0;i<1000000;++i)
	// {
	// 	timer.start_timer(200+rand()%1000,ptest->make_ptr());
	// }
	// auto cur1 = now();
	// timer.start_timer(1000,ptest->make_ptr());
	// printf("insert cose:%d,%d",now()-cur1,cur1-cur);
	// return time(NULL);
	// return 0;
	// while(1)
	// {
	// 	timer.update();
	// 	if(timer.pqueue->empty())
	// 		break;
	// }
	// auto end = now();
	// printf("exiting..");
	// printf("cost:%d\n",end-cur);
	return 0;
}