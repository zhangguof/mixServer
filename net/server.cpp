#include "server.hpp"

TcpServer::TcpServer()
{
	state = STOP;
	max_connect_id = 0;
	ploop = std::make_shared<EventLoop>();
}
void TcpServer::start()
{
	state = RUNNING;
	log_debug("start tcp server:%s,%d",server_ip.c_str(),
		server_port);
	acceptor->listen();
	
	ploop->do_loop();

}
void TcpServer::bind(std::string ip,int port)
{
	server_ip = ip;
	server_port = port;
	acceptor = std::make_shared<Acceptor>(ip,port,get_this());
	ploop->regist_handle(acceptor);

}
void TcpServer::new_connect(int fd)
{
	// auto p = std::shared_ptr<TcpStream>(new TcpStream(fd,ploop,this));
	++max_connect_id;
	auto pstream = std::make_shared<TcpStream>(fd,max_connect_id
												,get_this());
	ploop->regist_handle(pstream);
	//streams.push_back(pstream);
	streams[max_connect_id] = pstream;
	log_debug("new connect:%d",max_connect_id);

}
void TcpServer::close_connect(std::shared_ptr<TcpStream> pstream)
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


void EchoServer::handle_read(std::shared_ptr<TcpStream> pstream)
{
	log_debug("echo server handle_read!");
	int size = pstream->read_buf.size();
	char *pbuf = pstream->read_buf.read();
	pbuf[size] = '\0';
	printf("read str:%s,%d\n",pbuf,size);
	auto buf = std::make_shared<Buffer>();
	buf->append(pbuf,size);
	pstream->send(buf);
}