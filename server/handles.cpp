
#include "eventloop.hpp"
#include "server.hpp"
#include "handles.hpp"
#include "log.hpp"
#include "errors.hpp"


//class Acceptor
Acceptor::Acceptor(std::weak_ptr<TcpServer> _server)
{
	psocket = std::make_shared<Socket>();
	init(psocket->socket_fd,READ|ERROR);
	pserver = _server;
}
int Acceptor::listen(std::string ip, int port)
{
	log_debug("start listen!!");
	int n = psocket->bind(ip,port);
	if(n<0)
	{
		log_debug("bind error:%d,%s:%s",
			errno,get_error_msg(errno),strerror(errno));
		return n;
	}

	n = psocket->listen();
	if(n<0)
	{
		log_debug("listen error:%d,%s:%s",
			errno,get_error_msg(errno),strerror(errno)
			);
		return n;
	}
	return n;
}
void Acceptor::handle_read()
{
	int fd = psocket->accept();
	// tcpserver->new_connect(fd);
	get_server()->new_connect(fd);
	log_debug("Acceptor handle read:fd:%d",fd);
}
void Acceptor::handle_write(){}
void Acceptor::handle_error(){}
void Acceptor::close()
{
	//psocket->close();
	get_server()->shutdown();
}


//class TcpStream
TcpStream::TcpStream(int fd,
	int _connect_id,
	std::weak_ptr<TcpServer> _server):
	Handle(fd,READ|ERROR)
{
	psocket = std::make_shared<Socket>(fd);
	pread_buf = std::make_shared<Buffer>();
	pwrite_buf = std::make_shared<Buffer>();
	pserver = _server;
	connect_id  = _connect_id;


}
void TcpStream::handle_read()
{
	log_debug("TcpStream::handle_read:%d,fd:%d",connect_id,psocket->socket_fd);
	// Buffer buf;
	int n = psocket->recv(pread_buf);
	if(n==0 || (n<0 && errno!=EAGAIN && errno!=EWOULDBLOCK))
	{
		if(n<0)
			log_debug("error,no:%d,msg:%s,%s\n",errno,
				get_error_msg(errno),strerror(errno)
				);
		else
			log_debug("peer close!!!");
		// log_debug("test:%d,%d,%s",n,errno,get_error_msg(errno));
		close();
		return;
	}
	get_server()->handle_read(get_this());
}
void TcpStream::handle_write(){
	log_debug("TcpStream::handle_write:%d",connect_id);
	assert(pwrite_buf);
	int size = pwrite_buf->readable_size();
	int n = psocket->send(pwrite_buf);
	if(n<0 && errno!=EAGAIN && errno!=EWOULDBLOCK)
	{
		log_debug("error:%d,no:%d,msg:%s,%s\n",n,errno,
			get_error_msg(errno),strerror(errno));
		close();
		return;
	}
	int remain_size = pwrite_buf->readable_size();
	if(remain_size<=0)
	{
		disable_write();
	}

	log_debug("write_success!:has_send:%d,remain:%d",
		size - remain_size, remain_size);
	
}
void TcpStream::handle_error(){
	log_debug("TcpStream::handle_error:%d:%d,%s,%s",
		psocket->socket_fd,errno,get_error_msg(errno),
		strerror(errno)
		);
}

void TcpStream::send(std::string s)
{
	pwrite_buf->write(s);
	enable_write();
}
void TcpStream::send(const char* p,int size)
{
	pwrite_buf->write(p,size);
	enable_write();
}
void TcpStream::send(std::shared_ptr<std::string> ps)
{
	send(*ps);
}

void TcpStream::close()
{
	//psocket->close();
	get_server()->close_connect(get_this());
	
}

TcpStream::~TcpStream(){
	log_debug("release stream:%d",connect_id);

}


