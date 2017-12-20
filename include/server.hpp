#ifndef _SERVER_H
#define _SERVER_H

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

class TcpServer:public std::enable_shared_from_this<TcpServer>
{
public:
	TcpServer();
	void start();
	void bind(std::string ip,int port);
	void new_connect(int fd);
	void close_connect(std::shared_ptr<TcpStream> pstream);

	virtual void handle_read(std::shared_ptr<TcpStream> pstream)
	{
		log_debug("tcp server handle_read!");
	}
	inline std::shared_ptr<TcpServer> get_this()
	{
		return shared_from_this();
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

class EchoServer:public TcpServer
{
	void handle_read(std::shared_ptr<TcpStream> pstream);
};

#endif