#include "server.hpp"
#include <signal.h>
#include <iostream>
#include <functional>
#include "handles.hpp"

class IgnoreSigPipe
{
public:
	IgnoreSigPipe()
	{
		::signal(SIGPIPE,SIG_IGN);
	}
};
IgnoreSigPipe ignore_sigpipe;

TcpServer::TcpServer()
{
	state = STOP;
	max_connect_id = 0;
	// ploop = std::make_shared<EventLoop>();
}
int TcpServer::start()
{
	state = RUNNING;
	log_debug("start tcp server:%s,%d",server_ip.c_str(),
		server_port);
	assert(acceptor);
	int n = acceptor->listen(server_ip,server_port);
	if(n<0)
	{
		log_debug("TcpServer start error!!!");
	}
	return n;
	// ploop->do_loop();

}
void TcpServer::bind(std::string ip,int port)
{
	server_ip = ip;
	server_port = port;
	acceptor = std::make_shared<Acceptor>(get_this());
	ploop->regist_handle(acceptor);

}
void TcpServer::new_connect(int fd)
{
	// auto p = std::shared_ptr<TcpStream>(new TcpStream(fd,ploop,this));
	++max_connect_id;
	// auto pstream = STD::make_shared<TcpStream>(fd,max_connect_id
	// 											,get_this());
	auto pstream = ptstream_t(new TcpStream(fd,max_connect_id,get_this()));

	ploop->regist_handle(pstream);
	//streams.push_back(pstream);
	streams[max_connect_id] = pstream;
	log_debug("new connect:%d",max_connect_id);

}
void TcpServer::handle_read(pttcpstream_t pstream)
{
	log_debug("tcp server handle_read!");
}
void TcpServer::close_connect(pttcpstream_t pstream)
{
	int con_id = pstream->connect_id;
	auto it = streams.find(con_id);
	assert(it!=streams.end());

	log_debug("close connect:%d,total:%d",pstream->connect_id,streams.size());

	ploop->unregist_handle(pstream);
	streams.erase(it);
}
void TcpServer::shutdown()
{
	log_debug("shutdown server....");
	ploop->shutdown();
	// acceptor->close();
}

void EchoServer::handle_read(pttcpstream_t pstream)
{
	log_debug("echo server handle_read!");
	int size = pstream->pread_buf->readable_size();
	char *pbuf = pstream->pread_buf->read();
	pbuf[size] = '\0';
	printf("read str:%s,%d\n",pbuf,size);
	pstream->send(pbuf,size);
}

void EchoServer::new_connect(int fd)
{
	TcpServer::new_connect(fd);
	auto pstream = streams[max_connect_id];
	std::string s = "hello world!";
	
	// auto pwbuf = std::make_shared<Buffer>();
	// pwbuf->write(s);
	pstream->send(s);
}



