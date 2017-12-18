#include <stdio.h>
#include "server.hpp"
// #include "channel.hpp"
#include <string>
#include <iostream>
#include <memory>
#include <time.h>
#include "log.hpp"

const int max_fds = 1024;


int main(int argn,char** argv)
{
	auto cpp_server = std::make_shared<TcpServer>();
	std::string ip = "127.0.0.1";
	int port = 8889;
	cpp_server->bind(ip,port);

	cpp_server->start();


	// delete cpp_server;
	return 0;
}