#ifndef _SERVER_H
#define _SERVER_H


// #include "channel.hpp"
#include "socket.hpp"
#include "handle.hpp"
#include "handles.hpp"
#include "eventloop.hpp"
#include "log.hpp"


#include <unistd.h>
#include <map>
#include <vector>
#include <memory>
#include <functional>
#include <string>

enum SERVER_STATE
{
	STOPING,
	RUNNING,
	STOP,
};

class TcpServer
{
public:
	TcpServer()
	{
		state = STOP;
		ploop = std::make_shared<EventLoop>();
		// loop = std::shared_ptr<EventLoop>(new EventLoop());

	}
	void start()
	{
		state = RUNNING;
		log_debug("start tcp server:%s,%d",server_ip.c_str(),
			server_port);
		acceptor->listen();
		
		ploop->do_loop();

	}
	void bind(std::string ip,int port)
	{
		server_ip = ip;
		server_port = port;
		// acceptor = std::make_shared<Acceptor>(ip,port,loop,this);
		acceptor = std::shared_ptr<Acceptor>(new Acceptor(ip,port,ploop,this));
		ploop->regist_handle(acceptor);
		

	}
	void new_connect(int fd)
	{
		// auto p = std::shared_ptr<TcpStream>(new TcpStream(fd,ploop,this));
		auto pstream = std::make_shared<TcpStream>(fd,ploop,this);
		ploop->regist_handle(pstream);
		streams.push_back(pstream);
	}

	std::string server_ip;
	int server_port;
	SERVER_STATE state;
	std::shared_ptr<EventLoop> ploop;
	std::shared_ptr<Acceptor> acceptor;
	std::weak_ptr<TcpServer> _this;
	std::vector<std::shared_ptr<TcpStream> > streams;



};

#endif