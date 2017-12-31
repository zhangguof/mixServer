#ifndef _SERVER_H
#define _SERVER_H

#include "socket.hpp"
#include "handle.hpp"
#include "handles.hpp"
#include "eventloop.hpp"
#include "log.hpp"
#include "proto.hpp"
#include "services.hpp"

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
	typedef std::shared_ptr<TcpServer> pttcpsevert_t;
	typedef std::shared_ptr<TcpStream> pttcpstream_t;
	typedef std::shared_ptr<Msg> ptmsg_t;

	TcpServer();
	int start();
	void bind(std::string ip,int port);
	void close_connect(std::shared_ptr<TcpStream> pstream);
	
	virtual void handle_read(std::shared_ptr<TcpStream> pstream);
	virtual void new_connect(int fd);

	void set_loop(EventLoop::ptloop_t _ploop)
	{
		ploop = _ploop;
	}
	inline std::shared_ptr<TcpServer> get_this()
	{
		return shared_from_this();
	}
	inline EventLoop::ptloop_t get_loop()
	{
		return ploop;
	}
	void shutdown();

	std::string server_ip;
	int server_port;
	SERVER_STATE state;
	std::shared_ptr<EventLoop> ploop;
	std::shared_ptr<Acceptor> acceptor;
	int max_connect_id;
	std::map<int, std::shared_ptr<TcpStream> > streams;

};
class MsgServer:public TcpServer
{
public:
	typedef std::shared_ptr<Msg> ptmsg_t;
	typedef std::shared_ptr<ProtoMsg> ptpmsg_t;
	MsgServer();
	void new_connect(int fd);
	void handle_read(pttcpstream_t pstream);
	void handle_msg(int conn_id,const ptmsg_t&  pmsg);
	void send_msg(int conn_id, const ptmsg_t& pmsg);
private:
	std::map<int,ptmsg_t> conn_msgs;

};

class EchoServer:public TcpServer
{
	void new_connect(int fd);
	void handle_read(pttcpstream_t pstream);
};


#endif