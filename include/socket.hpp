#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <cassert>
#include <vector>
#include <algorithm>
#include "log.hpp"
#include "errors.hpp"

typedef struct sockaddr SA;

const int LISTENNQ = 65535;
const int BUFFER_SIZE = 4096;

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
	unsigned int get_sa_len()
	{
		return sizeof(SA);
	}
	std::string ip;
	unsigned short port;
	int family;
	struct sockaddr_in s_addr;
};

class Buffer
{
public:
	Buffer()
	{
		start_idx = 0;
		end_idx  = 0;
		date.resize(BUFFER_SIZE);

	}
	int size()
	{
		return end_idx - start_idx;
	}
	void clear()
	{
		start_idx = 0;
		end_idx = 0;
	}
	void make_space(int len)
	{
		if(size()+len < date.capacity())
			date.resize(size()+len);
	}
	void append(char* buf,int len)
	{
		make_space(len);
		std::copy(buf,buf+len,end());
		end_idx+=len;

	}
	char* end()
	{
		return begin() + end_idx;
	}
	char *begin()
	{
		return &(*date.begin());
	}
	char* read()
	{
		char* r = &(*(date.begin()+start_idx));
		clear();
		return r;
	}
	char* read(int n)
	{
		if(start_idx>=end_idx)
			return 0;
		char* r = &(*(date.begin()+start_idx));
		start_idx += n;
		return r;
	}
	char* get_buf()
	{
		char* r = &(*(date.begin()+start_idx));
		return r;
	}
	~Buffer(){
	}
private:
	std::vector<char> date;
	int start_idx;
	int end_idx;
	
};

class Socket
{
public:
	Socket(int fd)
	{
		socket_fd = fd;
	}
	Socket(){
		socket_fd = -1;
	}
	void bind(std::string ip,unsigned short port)
	{
		socket_fd = socket(AF_INET,SOCK_STREAM,0);
		assert(socket_fd != -1);
		addr.bind(ip,port);
		::bind(socket_fd,addr.get_sa(),addr.get_sa_len());
		set_nonblock();
	}
	void listen()
	{
		assert(socket_fd!=-1);
		::listen(socket_fd,LISTENNQ);
	}
	int recv(Buffer& buf){
		char rbuf[BUFFER_SIZE];
		int n;
		while(1)
		{
			n=::recv(socket_fd,rbuf,BUFFER_SIZE,0);
			// log_debug("read once:%d",n);
			if(n<=0)
				return n;
			buf.append(rbuf,n);
		}
		return n;

	}
	int accept()
	{
		SA sock_addr;
		socklen_t add_len = sizeof(sock_addr);
		int fd = ::accept(socket_fd,&sock_addr,&add_len);
		assert(fd!=-1);

		return fd;
	}


	int send(std::shared_ptr<Buffer> pbuf){
		int n;
		int size = pbuf->size();
		int send_size = 0;
		while(size>0)
		{
			send_size = BUFFER_SIZE<size?BUFFER_SIZE:size;
			n = ::send(socket_fd,pbuf->get_buf(),send_size,0);
			// log_debug("send once:%d",n);
			if(n<0)
				return n;
			size -= n;
			pbuf->read(n);

		}

		return n;
	}
	void set_nonblock()
	{
		if(socket_fd!=-1)
			fcntl(socket_fd, F_SETFL, O_NONBLOCK);
	}
	void close()
	{
		assert(socket_fd!=-1);
		::close(socket_fd);
		socket_fd = -1;
	}
	~Socket()
	{
		log_debug("release socket fd:%d",socket_fd);
		if(socket_fd!=-1)
			close();
	}
	int socket_fd;
	SockAddr addr;


};


#endif