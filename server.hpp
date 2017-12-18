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
		max_connect_id = 0;
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
		acceptor = std::make_shared<Acceptor>(ip,port,ploop,this);
		// acceptor = std::shared_ptr<Acceptor>(new Acceptor(ip,port,ploop,this));
		ploop->regist_handle(acceptor);
		

	}
	void new_connect(int fd)
	{
		// auto p = std::shared_ptr<TcpStream>(new TcpStream(fd,ploop,this));
		++max_connect_id;
		auto pstream = std::make_shared<TcpStream>(fd,max_connect_id
													,ploop,this);
		ploop->regist_handle(pstream);
		//streams.push_back(pstream);
		streams[max_connect_id] = pstream;
		log_debug("new connect:%d",max_connect_id);

	}
	void close_connect(int con_id)
	{
		auto it = streams.find(con_id);
		if(it!=streams.end())
		{
			auto pstream = (*it).second;
			log_debug("close connect:%d",pstream->connect_id);

			ploop->unregist_handle(pstream);
			streams.erase(it);
		}
	}

	std::string server_ip;
	int server_port;
	SERVER_STATE state;
	std::shared_ptr<EventLoop> ploop;
	std::shared_ptr<Acceptor> acceptor;
	// std::weak_ptr<TcpServer> _this;
	int max_connect_id;
	std::map<int, std::shared_ptr<TcpStream> > streams;



};

#endif