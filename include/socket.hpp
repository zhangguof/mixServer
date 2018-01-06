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
#include "buffer.hpp"

typedef struct sockaddr SA;

const int LISTENNQ = 65535;
const int BUFFER_SIZE = 4096;

class SockAddr
{
public:
	SockAddr();
	SockAddr(struct sockaddr_in *sock_addr);
	SockAddr(SA *sock_addr);
	SockAddr(std::string _ip, unsigned short _port);
	void bind(std::string _ip, unsigned short _port);
	void bind(struct sockaddr_in *sock_addr);
	void bind(SA* sock_addr);
	struct sockaddr_in* get_sockin_addr();
	SA* get_sa();
	unsigned int get_sa_len();

	std::string ip;
	unsigned short port;
	int family;
	struct sockaddr_in s_addr;
};



class Socket
{
public:
	Socket(int fd);
	Socket();
	int bind(std::string ip,unsigned short port);
	int listen();
	int accept();

	int recv(std::shared_ptr<Buffer> pbuf);
	int recv(int size);

	int send(std::shared_ptr<Buffer> pbuf);

	int connect(std::string ip,unsigned short port);
	int connect();

	void set_nonblock();
	void get_opt(int level,int optname,void *optval,socklen_t *optlen);
	void close();
	~Socket();

	int socket_fd;
	SockAddr addr;

};


#endif