#ifndef _SERVER_H
#define _SERVER_H value
#include <string>
#include "channel.hpp"
#include <unistd.h>
#include <map>
#include <vector>
#include <memory>
#include <functional>

enum SERVER_STATE
{
	STOPING,
	RUNNING,
	STOP,
};

// enum EVENT
// {
// 	READ = 0x01, 
// 	WRITE = 0x02,
// 	RW = READ|WRITE,	
// };
#define READ 0x01
#define WRITE 0x02

typedef void (*EventCB)();

class Server
{
public:
	Server(){
		using namespace std::placeholders;
		state = STOP;
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_ZERO(&exceptfds);
		FD_ZERO(&fd_all);
		listen_ch = std::make_shared<Channel>();
		auto ptr = std::mem_fn(&Server::newConnect,_1);
		// listen_ch->set_new_connect_cb();
		channels.push_back(listen_ch);
		listen_ch->idx = channels.size()-1;


	}
	void start();
	void stop();
	void bind(std::string ip,int port);
	void bind();

	void newConnect()
	{

	}

	void fd_set(std::shared_ptr<Channel> ch,int e)
	{
		fd_set(ch->socket_fd,e);
	}

	void fd_set(int fd,int e)
	{
		if((e & READ))
		{
			FD_SET(fd,&readfds);
			//std::cout<<"set fd:"<<fd<<" "<<std::endl;
		}
		if((e & WRITE))
		{
			FD_SET(fd,&writefds);
		}
	}
	bool fd_isset(ptrChannel pch,struct fd_set* fds)
	{
		return fd_isset(pch->socket_fd,fds);
	}
	bool fd_isset(int fd,struct fd_set* fds)
	{
		return FD_ISSET(fd,fds);
	}
	void fd_clear(int fd,int e)
	{

	}

	std::string server_ip;
	int server_port;
	SERVER_STATE state;
	std::shared_ptr<Channel> listen_ch;
	std::map<int, std::shared_ptr<Channel> > channel_map;
	std::vector<std::shared_ptr<Channel> > channels;
	struct fd_set readfds;
	struct fd_set writefds;
	struct fd_set exceptfds;
	struct fd_set fd_all;
};

#endif