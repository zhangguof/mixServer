#ifndef _HANDLES_H_
#define _HANDLES_H_
#include "handle.hpp"
#include "socket.hpp"

class TcpServer;
class Acceptor:public Handle
{
public:
	Acceptor(std::weak_ptr<TcpServer> _server);
	int listen(std::string ip, int port);
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
	void send(std::string s);
	void send(const char*p, int size);
	void send(std::shared_ptr<std::string> ps);
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

	std::shared_ptr<Buffer> pwrite_buf;
	std::shared_ptr<Buffer> pread_buf;
	int connect_id;
	bool closed;
};

#endif