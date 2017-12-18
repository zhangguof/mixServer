
#include "eventloop.hpp"
#include "server.hpp"
#include "handles.hpp"
#include "log.hpp"


//class Acceptor
Acceptor::Acceptor(std::string ip, int port,
	std::shared_ptr<EventLoop> loop,
	TcpServer* _tcpserver)
	:Handle(loop)
{
	socket = std::make_shared<Socket>();
	socket->bind(ip,port);
	set_fd(socket->socket_fd,READ|ERROR);
	tcpserver = _tcpserver;
}
void Acceptor::listen()
{
	log_debug("start listen!!");
	socket->listen();
}
void Acceptor::handle_read()
{
	int fd = socket->accept();
	tcpserver->new_connect(fd);
	log_debug("Acceptor handle read:%d",fd);
}
void Acceptor::handle_write(){}
void Acceptor::handle_error(){}


//class TcpStream
TcpStream::TcpStream(int fd,
	std::shared_ptr<EventLoop> loop,TcpServer* _server):
	Handle(fd,READ|ERROR,loop)
{
	socket = std::make_shared<Socket>(fd);
	pserver = _server;

}
void TcpStream::handle_read()
{
	log_debug("TcpStream::handle_read");
	Buffer buf;
	socket->recv(buf);
	char *pbuf = buf.read();
	pbuf[buf.size()] = '\0';
	printf("out:%s\n",pbuf);
}
void TcpStream::handle_write(){
	log_debug("TcpStream::handle_write:%d,e:%0x,%0x",get_fd(),events,revents);

}
void TcpStream::handle_error(){
	log_debug("TcpStream::handle_error");
}

void TcpStream::send(Buffer& buf)
{
	enable_write();
	

}

TcpStream::~TcpStream(){}



