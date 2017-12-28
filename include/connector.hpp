#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_
#include "types.hpp"
#include "eventloop.hpp"
#include "handle.hpp"
#include "socket.hpp"
#include "log.hpp"
#include "proto.hpp"

#include <memory>
#include <iostream>

class Client;
enum STATUS
{
	CLOSED = 0,
	CONNECTING = 1,
	CONNECTED = 2,
};

class Connector:public Handle
{
public:

	typedef std::shared_ptr<Buffer> ptbuffer_t;
	typedef std::shared_ptr<Connector> ptconnector_t;
public:
	Connector(std::weak_ptr<Client> _client);

	int connect(std::string ip,port_t port);
	int _connect();
	void handle_read();
	void handle_write();
	void handle_error();
	void send(std::string s);
	void send(std::shared_ptr<std::string> ps);
	void send(const char* p,int size);
	void close();
	int get_status()
	{
		return status;
	}
	std::shared_ptr<Client> get_client()
	{
		auto p = pclient.lock();
		assert(p);
		return p;
	}
	ptconnector_t get_this()
	{
		return std::static_pointer_cast<Connector>(
			shared_from_this());
	}
	SockAddr get_addr()
	{
		return psocket->addr;
	}
	~Connector()
	{
		log_debug("release Connector!");
	}
	std::shared_ptr<Buffer> pread_buf;
	std::shared_ptr<Buffer> pwrite_buf;
private:
	std::shared_ptr<Socket> psocket;
	std::weak_ptr<Client> pclient;
	int status;
	std::string ip;
	unsigned short port;
	
	
	
};

class Client:public std::enable_shared_from_this<Client>
{
public:
	static const int max_retry_cout = 10;
	static const int retry_timeout = 100*10;//10s
	typedef std::shared_ptr<Msg> ptmsg_t;
	Client();
	~Client();
	int start_connect(std::string ip, port_t port);
	int _start_connect();
	void on_connected();
	void do_timeout_check();
	void _re_connect(Timer::pttimer_t ptimer);
	void on_read();
	void on_close();
	void on_msg(ptmsg_t pmsg);
	void send_msg(ptmsg_t pmsg);
	void send_msg(const char*p, int size);
	void do_loop()
	{
		ploop->do_loop();
	}
	void close()
	{
		ploop->shutdown();
	}

	inline std::shared_ptr<Client> get_this()
	{
		return shared_from_this();
	}
	inline int get_status()
	{
		return status;
	}

private:
	int retry_count;
	std::shared_ptr<EventLoop> ploop;
	std::shared_ptr<Connector> pconn;
	ptmsg_t pmsg;
	int msg_reading;
	int status;
	std::string ip;
	port_t port;
	// Proto* proto;

};


#endif

