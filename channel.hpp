#ifndef _CHANNEL_H
#define _CHANNEL_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cassert>
#include <string.h>
#include <string>
#include <iostream>
#include <memory>


// sockfd = socket(PF_INET, SOCK_STREAM, 0);

typedef struct sockaddr SA;
#define LISTENNQ (65535)

class EventLoop;

class SockAddr
{
public:
	SockAddr(){}
	SockAddr(struct sockaddr_in *sock_addr)
	{
		bind(sock_addr);
	}
	SockAddr(SA *sock_addr)
	{
		bind(sock_addr);
	}
	SockAddr(std::string _ip, unsigned short _port)
	{
		bind(_ip,_port);
	}
	void bind(std::string _ip, unsigned short _port)
	{
		ip = _ip;
		port = _port;
		s_addr.sin_family = AF_INET;
		s_addr.sin_port = htons(_port);
		if(_ip.length()==0)
		{
			s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		}
		else
		{
			 inet_pton(AF_INET,_ip.c_str(),&s_addr.sin_addr);
		}
	}
	void bind(struct sockaddr_in *sock_addr)
	{
		char addr_buf[20];
		ip = inet_ntop(AF_INET,&(sock_addr->sin_addr),addr_buf,sizeof(addr_buf));
		port = ntohs(sock_addr->sin_port);
		family = sock_addr->sin_family;
		s_addr = *sock_addr;
	}
	void bind(SA* sock_addr)
	{
		bind((struct sockaddr_in*) sock_addr);
	}
	struct sockaddr_in* get_sockin_addr()
	{
		return &s_addr;
	}
	SA* get_sa()
	{
		return (SA*)(&s_addr);
	}
	std::string ip;
	unsigned short port;
	int family;
	struct sockaddr_in s_addr;
};

class Channel;
typedef std::shared_ptr<Channel> ptrChannel;
typedef void (*EventCB)();

class Channel
{
public:

	typedef void (*newConnectCB)();
	typedef void (*readHanleCB)();
	typedef void (*writeHandleCB)();

	Channel(){
		socket_fd = -1;
		idx = -1;
	}
	Channel(int fd,SA *sock_addr)
	{
		socket_fd = fd;
		addr.bind(sock_addr);
		assert(socket_fd != -1);
		set_nonblock();
	}
	// bool connect(){}
	bool bind(std::string ip,unsigned short port){
		socket_fd = socket(AF_INET,SOCK_STREAM,0);
		assert(socket_fd != -1);
		addr.bind(ip,port);
		set_nonblock();

		::bind(socket_fd,addr.get_sa(),sizeof(sockaddr_in));
		//std::cout<<"do bind:"<<socket_fd<<":"<<ip<<":"<<port<<std::endl;

		return true;
	}
	bool bind(unsigned short port){
		addr.bind("",port);
		::bind(socket_fd,addr.get_sa(),sizeof(sockaddr_in));

		return true;
	}
	void listen()
	{
		::listen(socket_fd,LISTENNQ);
	}

	std::shared_ptr<Channel> accept()
	{
		//std::cout<<"start do accept!"<<std::endl;

		SA sock_addr;
		socklen_t add_len = sizeof(sock_addr);
		int fd = ::accept(socket_fd,&sock_addr,&add_len);
		assert(fd!=-1);
		ptrChannel ch(new Channel(fd,&sock_addr));
		//std::cout<<"new Channel!!"<<fd<<":"<<ch->addr.ip<<":"<<ch->addr.port<<std::endl;
		// return *ch;
		return ch;
	}
	int read()
	{
		int size,n;
		size = 0;
		char* pbuf = rbuf;
		while((n = ::read(socket_fd,pbuf,4096)) > 0 && pbuf<rbuf+4096)
		{
			pbuf += n;
			size += n;
		}
		pbuf[size]='\0';
		return size;
	}

	void set_nonblock()
	{
		if(socket_fd!=-1)
			fcntl(socket_fd, F_SETFL, O_NONBLOCK);
	}
	void set_new_connect_cb(EventCB cb)
	{
		new_connect_cb = cb;
	}
	void set_read_hanle_cb(EventCB cb)
	{
		read_handle = cb;
	}
	void set_write_handle_cb(EventCB cb)
	{
		write_handle = cb;
	}

	SockAddr addr;
	char rbuf[4096];
	char wbuf[4096];
	int socket_fd;
	int idx;
private:	
	newConnectCB new_connect_cb;
	readHanleCB read_handle;
	writeHandleCB write_handle;

	
	
};

#endif