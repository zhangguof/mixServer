#include <stdio.h>
#include "server.hpp"
#include <string>
#include <iostream>
#include <memory>
#include <time.h>
#include "log.hpp"

class TimerTest:std::enable_shared_from_this<TimerTest>
{
	void test(u32 _id,Timer::pttimer_t pttimer)
	{
		log_debug("===test");
		pttimer->start_timer(200,
			Timer::make_handle(shared_from_this(),&TimerTest::test));
	}
};


int main(int argn,char** argv)
{
	auto cpp_server = std::make_shared<MsgServer>();
	auto ploop = cpp_server->get_loop();

	std::string ip = "127.0.0.1";
	int port = 8889;
	if(argn>1)
	{
		// printf("argn:%d,%s",argn,argv[argn-1]);
		port = atoi(argv[1]);
	}
	// ploop->start_timer(200,TimerHandle<TimerTest>::make)
	cpp_server->bind(ip,port);
	cpp_server->start();


	// delete cpp_server;
	return 0;
}