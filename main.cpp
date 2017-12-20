#include <stdio.h>
#include "server.hpp"
#include <string>
#include <iostream>
#include <memory>
#include <time.h>
#include "log.hpp"


int main(int argn,char** argv)
{
	auto cpp_server = std::make_shared<EchoServer>();
	std::string ip = "127.0.0.1";
	int port = 8889;
	if(argn>1)
	{
		// printf("argn:%d,%s",argn,argv[argn-1]);
		port = atoi(argv[1]);
	}
	cpp_server->bind(ip,port);
	cpp_server->start();


	// delete cpp_server;
	return 0;
}