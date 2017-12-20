#ifndef _HANDLES_H_
#define _HANDLES_H_
#include "handle.hpp"
#include "socket.hpp"

class TcpServer;
class Acceptor:public Handle
{
public:
	Acceptor(std::string ip, int port,
		std::weak_ptr<TcpServer> _server);
	void listen();
	void handle_read();
	void handle_write();
	void handle_error();
	void close();
	inline std::shared_ptr<TcpServer> get_server()
	{
		auto p = pserver.lock();
		assert(p);
		return p;
	}
	std::shared_ptr<Socket> psocket;
	std::weak_ptr<TcpServer> pserver;

};

class TcpStream:public Handle
{
public:
	TcpStream(int fd,
		int connect_id,
		std::weak_ptr<TcpServer> _server);
	~TcpStream();
	void handle_read();
	void handle_write();
	void handle_error();
	void send(std::shared_ptr<Buffer>  pbuf);
	void close();
	std::shared_ptr<TcpStream> get_this()
	{
			return std::static_pointer_cast<TcpStream>(
				shared_from_this());
	}
	inline std::shared_ptr<TcpServer> get_server()
	{
		auto p = pserver.lock();
		assert(p);
		return p;
	}
	std::shared_ptr<Socket> psocket;
	std::weak_ptr<TcpServer> pserver;
	// Buffer write_buf;
	std::shared_ptr<Buffer> pwrite_buf;
	Buffer read_buf;
	int connect_id;
};

#endif