#include <stdio.h>
#include "server.hpp"
#include "channel.hpp"
#include <string>
#include <iostream>
#include <memory>
#include <time.h>
#include "log.hpp"

const int max_fds = 1024;

void Server::start(){
	state = RUNNING;
	//std::cout<<"start server:"<<server_ip<<":"<<server_port<<std::endl;
	// log_debug("test log!!!");
	log_debug("start sever:%s:%d",server_ip.c_str(),server_port);
	listen_ch->listen();
	struct timeval zero_sec;
	zero_sec.tv_sec = 0;
	zero_sec.tv_usec=0;
	// std::cout<<"listen fd:"<<listen_ch->socket_fd<<std::endl;
	struct fd_set rfds;

	while(1)
	{
		int size = channels.size();
		rfds = readfds;
		// std::cout<<"num_fds:"<<size<<std::endl;
		select(max_fds,&rfds,NULL,NULL,&zero_sec);
		//Channel &ch = listen_ch.accept();
		for(int i=0;i<size;++i)
		{
			// int fd = channels[i]->socket_fd;
			auto ch = channels[i];
			if(fd_isset(ch,&rfds))
			{
				// std::cout<<"read events:"<<fd<<std::endl;
				if(i == listen_ch->idx)
				{
					
					std::shared_ptr<Channel> new_ch = listen_ch->accept();
					channels.push_back(new_ch);
					new_ch->idx = channels.size()-1;
					fd_set(new_ch,READ | WRITE);
					//std::cout<<"new connect:"<<new_ch->addr.ip<<":"<<new_ch->addr.port<<std::endl;
					log_debug("new connect:%s,%d",new_ch->addr.ip.c_str(),new_ch->addr.port);
				}
				else
				{
					// auto ch = channels[i];
					int n = ch->read();
					//std::cout<<"read :"<<n<<"==="<<ch->rbuf<<std::endl;
					log_debug("read:%d,%s",n,ch->rbuf);
				}
			}
		}
		// sleep(2);


	}
}

void Server::bind(std::string ip,int port)
{
	server_ip = ip;
	server_port = port;	
	listen_ch->bind(ip,port);
	fd_set(listen_ch,READ);
}


int main(int argn,char** argv)
{
	auto cpp_server = std::make_shared<Server>();
	std::string ip = "127.0.0.1";
	int port = 8888;
	cpp_server->bind(ip,port);

	cpp_server->start();


	// delete cpp_server;
	return 0;
}