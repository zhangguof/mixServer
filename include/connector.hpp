#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_
#include "eventloop.hpp"
#include "handle.hpp"
#include "socket.hpp"
#include "log.hpp"
#include "proto.hpp"
#include <memory>
#include <iostream>

class Client;

class Connector:public Handle
{
public:
	enum STATUS
	{
		CLOSED = 0,
		CONNECTING = 1,
		CONNECTED = 2,
	};
	typedef std::shared_ptr<Buffer> ptbuffer_t;
public:
	Connector(std::weak_ptr<Client> _client);

	void connect(std::string ip,unsigned short port);
	void handle_read();
	void handle_write();
	void handle_error();
	void send(std::string s);
	void send(std::shared_ptr<std::string> ps);
	void send(const char* p,int size);
	void close();
	std::shared_ptr<Client> get_client()
	{
		auto p = pclient.lock();
		assert(p);
		return p;
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
	
	
};

class Client:public std::enable_shared_from_this<Client>
{
public:
	typedef std::shared_ptr<Msg> ptmsg_t;
	Client()
	{
		ploop = std::make_shared<EventLoop>();
		// log_debug("after make Connector");
		msg_reading = false;
	}
	void start_connect(std::string ip, unsigned short port);
	void on_connected();
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

private:
	std::shared_ptr<EventLoop> ploop;
	std::shared_ptr<Connector> pconn;
	ptmsg_t pmsg;
	bool msg_reading;

};


#endif

