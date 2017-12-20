
#include "eventloop.hpp"
#include "server.hpp"
#include "handles.hpp"
#include "log.hpp"
#include "errors.hpp"


//class Acceptor
Acceptor::Acceptor(std::string ip, int port,
	std::weak_ptr<TcpServer> _server)
{
	psocket = std::make_shared<Socket>();
	psocket->bind(ip,port);
	init(psocket->socket_fd,READ|ERROR);
	pserver = _server;
}
void Acceptor::listen()
{
	log_debug("start listen!!");
	psocket->listen();
}
void Acceptor::handle_read()
{
	int fd = psocket->accept();
	// tcpserver->new_connect(fd);
	get_server()->new_connect(fd);
	log_debug("Acceptor handle read:%d",fd);
}
void Acceptor::handle_write(){}
void Acceptor::handle_error(){}


//class TcpStream
TcpStream::TcpStream(int fd,
	int _connect_id,
	std::weak_ptr<TcpServer> _server):
	Handle(fd,READ|ERROR)
{
	psocket = std::make_shared<Socket>(fd);
	pserver = _server;
	connect_id  = _connect_id;


}
void TcpStream::handle_read()
{
	log_debug("TcpStream::handle_read:%d,fd:%d",connect_id,psocket->socket_fd);
	// Buffer buf;
	int n = psocket->recv(read_buf);
	if(n==0 || (n<0 && errno!=EAGAIN && errno!=EWOULDBLOCK))
	{
		if(n<0)
			log_debug("error,no:%d,msg:%s\n",errno,get_error_msg(errno));
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
	int size = pwrite_buf->size();
	log_debug("===,buf size:%d",size);
	int n = psocket->send(pwrite_buf);
	if(n==0 || (n<0 && errno!=EAGAIN && errno!=EWOULDBLOCK))
	{
		log_debug("error:,no:%d,msg:%s\n",n,errno,get_error_msg(errno));
		close();
		return;
	}
	log_debug("write_success!:%d",size);
	disable_write();
	// get_server()->update_event(connect_id);
	
}
void TcpStream::handle_error(){
	log_debug("TcpStream::handle_error:%d:%d,%s",psocket->socket_fd,errno,get_error_msg(errno));
}

void TcpStream::send(std::shared_ptr<Buffer> pbuf)
{
	enable_write();
	pwrite_buf = pbuf;

}

void TcpStream::close()
{
	psocket->close();
	get_server()->close_connect(get_this());
	
}

TcpStream::~TcpStream(){
	log_debug("release stream:%d",connect_id);

}



