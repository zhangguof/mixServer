
#include "eventloop.hpp"
#include "server.hpp"
#include "handles.hpp"
#include "log.hpp"
#include "errors.hpp"


//class Acceptor
Acceptor::Acceptor(std::string ip, int port,
	std::shared_ptr<EventLoop> loop,
	TcpServer* _tcpserver)
	:Handle(loop)
{
	psocket = std::make_shared<Socket>();
	psocket->bind(ip,port);
	set_fd(psocket->socket_fd,READ|ERROR);
	tcpserver = _tcpserver;
}
void Acceptor::listen()
{
	log_debug("start listen!!");
	psocket->listen();
}
void Acceptor::handle_read()
{
	int fd = psocket->accept();
	tcpserver->new_connect(fd);
	log_debug("Acceptor handle read:%d",fd);
}
void Acceptor::handle_write(){}
void Acceptor::handle_error(){}


//class TcpStream
TcpStream::TcpStream(int fd,
	int _connect_id,
	std::shared_ptr<EventLoop> loop,TcpServer* _server):
	Handle(fd,READ|ERROR,loop)
{
	psocket = std::make_shared<Socket>(fd);
	pserver = _server;
	connect_id  = _connect_id;


}
void TcpStream::handle_read()
{
	log_debug("TcpStream::handle_read");
	// Buffer buf;
	int n = psocket->recv(read_buf);
	if(n==0 || (n<0 && errno!=EAGAIN && errno!=EWOULDBLOCK))
	{
		if(n<0)
			log_debug("error,no:%d,msg:%s\n",errno,get_error_msg(errno));
		else
			log_debug("peer close!!!");
		log_debug("test:%d,%d,%s",n,errno,get_error_msg(errno));
		close();
		return;
	}
	int size = read_buf.size();
	char *pbuf = read_buf.read();
	pbuf[size] = '\0';
	printf("read str:%s\n",pbuf);
}
void TcpStream::handle_write(){
	log_debug("TcpStream::handle_write");
	psocket->send(pwrite_buf);


}
void TcpStream::handle_error(){
	log_debug("TcpStream::handle_error");
}

void TcpStream::send(std::shared_ptr<Buffer> pbuf)
{
	enable_write();
	pwrite_buf = pbuf;
	//psocket->send(buf);
	// disable_write();

}

void TcpStream::close()
{
	pserver->close_connect(connect_id);
	psocket->close();
}

TcpStream::~TcpStream(){
	log_debug("release stream:%d",connect_id);

}



