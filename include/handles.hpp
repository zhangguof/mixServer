#ifndef _HANDLES_H_
#define _HANDLES_H_
#include "handle.hpp"
#include "socket.hpp"
#include "mem.hpp"
#include <dirent.h>

class TcpServer;
class TcpStream;
class Acceptor;

typedef STD::shared_ptr<TcpStream> ptstream_t;
typedef std::shared_ptr<Acceptor> ptacceptor_t;



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
	void send(const char* p, int size);
	void send(std::shared_ptr<std::string> ps);
	void close();
	ptstream_t get_this()
	{
			return STD::static_pointer_cast<TcpStream>(
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
	bool closing;
};

class FileHandle:public Handle
{
public:
	FileHandle(std::string& _file_path);
	FileHandle(const char* _file_path);
	inline void open();

	void handle_read(){}
	void handle_write(){}
	void handle_error();
	void close();
	void handle_modify();
	std::string file_path;
};


#endif