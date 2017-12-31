#include <stdio.h>
#include "server.hpp"
#include <string>
#include <iostream>
#include <memory>
#include <time.h>
#include "log.hpp"

void init_py(int argn,char** argv);
extern EventLoop* g_event_loop;

class TimerTest:public std::enable_shared_from_this<TimerTest>
{
public:
	void test(Timer::pttimer_t pttimer)
	{
		log_debug("===test");
		pttimer->_start_timer(200,
			Timer::make_handle(shared_from_this(),&TimerTest::test));
	}
};

int main(int argn,char** argv)
{
	auto msg_server = std::make_shared<MsgServer>();
	// auto echo_server = std::make_shared<EchoServer>();

	auto ploop = std::make_shared<EventLoop>();
	g_event_loop = ploop.get();
	// auto ploop = cpp_server->get_loop();
	msg_server->set_loop(ploop);
	// echo_server->set_loop(ploop);
	

	std::string ip = "127.0.0.1";
	int port = 8889;
	if(argn>1)
	{
		port = atoi(argv[1]);
	}
	log_debug("init py!!");
	init_py(argn,argv);
	// auto ptest = std::make_shared<TimerTest>();
	// ploop->start_timer(200,ptest,&TimerTest::test);

	msg_server->bind(ip,port);
	int n = msg_server->start();
	if(n<0)
	{
		return 0;
	}

	// echo_server->bind(ip,port+1);
	// echo_server->start();
	ploop->do_loop();
	// delete cpp_server;
	return 0;
}