#ifndef _HANDLES_H_
#define _HANDLES_H_
#include "handle.hpp"
#include "socket.hpp"

class TcpServer;
class Acceptor:public Handle
{
public:
	Acceptor(std::string ip, int port,
		std::shared_ptr<EventLoop> loop,
		TcpServer* _tcpserver);
	void listen();
	void handle_read();
	void handle_write();
	void handle_error();

	// int socket_fd;
	std::shared_ptr<Socket> socket;
	TcpServer* tcpserver;
	// std::shared_ptr<TcpServer> tcpserver;
};

class TcpStream:public Handle
{
public:
	TcpStream(int fd,
		std::shared_ptr<EventLoop> loop,
		TcpServer* _server);
	~TcpStream();
	void handle_read();
	void handle_write();
	void handle_error();
	void send(Buffer &buf);
	std::shared_ptr<Socket> socket;
	TcpServer* pserver;
};

#endif