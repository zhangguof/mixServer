#include "socket.hpp"


SockAddr::SockAddr(){}
SockAddr::SockAddr(struct sockaddr_in *sock_addr)
{
	bind(sock_addr);
}
SockAddr::SockAddr(SA *sock_addr)
{
	bind(sock_addr);
}
SockAddr::SockAddr(std::string _ip, unsigned short _port)
{
	bind(_ip,_port);
}
void SockAddr::bind(std::string _ip, unsigned short _port)
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
void SockAddr::bind(struct sockaddr_in *sock_addr)
{
	char addr_buf[20];
	ip = inet_ntop(AF_INET,&(sock_addr->sin_addr),addr_buf,sizeof(addr_buf));
	port = ntohs(sock_addr->sin_port);
	family = sock_addr->sin_family;
	s_addr = *sock_addr;
}
void SockAddr::bind(SA* sock_addr)
{
	bind((struct sockaddr_in*) sock_addr);
}

struct sockaddr_in* SockAddr::get_sockin_addr()
{
	return &s_addr;
}
SA* SockAddr::get_sa()
{
	return (SA*)(&s_addr);
}
unsigned int SockAddr::get_sa_len()
{
	return sizeof(SA);
}





Socket::Socket(int fd)
{
	assert(fd!=-1);
	socket_fd = fd;
//int getpeername(int socket, 
// struct sockaddr *restrict address,
// socklen_t *restrict address_len);
	unsigned int len = sizeof(SA);
	int n = ::getpeername(fd,addr.get_sa(),&len);
	addr.bind(addr.get_sa());
	assert(n==0);
		
}
Socket::Socket(){
	socket_fd = socket(AF_INET,SOCK_STREAM,0);
}
int Socket::bind(std::string ip,unsigned short port)
{
	// socket_fd = socket(AF_INET,SOCK_STREAM,0);
	assert(socket_fd != -1);
	addr.bind(ip,port);
	int n =::bind(socket_fd,addr.get_sa(),addr.get_sa_len());
	set_nonblock();
	return n;
}
int Socket::listen()
{
	assert(socket_fd!=-1);
	return ::listen(socket_fd,LISTENNQ);
}
int Socket::accept()
{
	SA sock_addr;
	socklen_t add_len = sizeof(sock_addr);
	int fd = ::accept(socket_fd,&sock_addr,&add_len);
	assert(fd!=-1);

	return fd;
}

int Socket::recv(std::shared_ptr<Buffer> pbuf){
	char rbuf[BUFFER_SIZE];
	int n;
	while(1)
	{
		n=::recv(socket_fd,rbuf,BUFFER_SIZE,0);
		// log_debug("read once:%d",n);
		if(n<=0)
			return n;
		pbuf->write(rbuf,n);
	}
}
int Socket::recv(int size)
{
	char rbuf[size];
	return ::recv(socket_fd,rbuf,size,0);
}

int Socket::send(std::shared_ptr<Buffer> pbuf){
	int n = 0;
	int size = pbuf->readable_size();
	int send_size = 0;
	char *buf = pbuf->get_buf();
	while(size>0)
	{
		send_size = BUFFER_SIZE<size?BUFFER_SIZE:size;
		n = ::send(socket_fd,buf,send_size,0);
		// log_debug("send once:%d",n);
		if(n<0)
			return n;
		size -= n;
		// pbuf->read(n);
		pbuf->has_read(n);
	}

	return n; //0=send all!
}

int Socket::connect(std::string ip,unsigned short port)
{
	// int connect(int sockfd, const struct sockaddr *addr,
    // socklen_t addrlen);
	assert(socket_fd!=-1);
	addr.bind(ip,port);
	
	set_nonblock();
	int n = ::connect(socket_fd,addr.get_sa(),sizeof(SA));
	return n;
	// ::bind(socket_fd,addr.get_sa(),addr.get_sa_len());	
}
int Socket::connect()
{
	assert(socket_fd!=-1);
	return ::connect(socket_fd,addr.get_sa(),sizeof(SA));
}

void Socket::set_nonblock()
{
	if(socket_fd!=-1)
		fcntl(socket_fd, F_SETFL, O_NONBLOCK);
}
void Socket::get_opt(int level,int optname,void *optval,socklen_t *optlen)
{
	// int getsockopt(int sockfd, int level, int optname,
    // void *optval, socklen_t *optlen);
	assert(socket_fd!=-1);
	::getsockopt(socket_fd,level,optname,optval,optlen);
}
void Socket::close()
{
	assert(socket_fd!=-1);
	::close(socket_fd);
	socket_fd = -1;
}
Socket::~Socket()
{
	log_debug("release socket fd:%d",socket_fd);
	if(socket_fd!=-1)
		close();
}
