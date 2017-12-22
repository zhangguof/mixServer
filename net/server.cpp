#include "server.hpp"
#include <signal.h>
#include <iostream>

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
	ploop = std::make_shared<EventLoop>();
}
void TcpServer::start()
{
	state = RUNNING;
	log_debug("start tcp server:%s,%d",server_ip.c_str(),
		server_port);
	assert(acceptor);
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
	// auto buf = std::make_shared<Buffer>();
	// buf->write(pbuf,size);
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


void MsgServer::handle_read(pttcpstream_t pstream)
{
	log_debug("msg server handle_read!");
	int conn_id = pstream->connect_id;
	assert(streams.find(conn_id)!=streams.end());

	ptmsg_t pmsg = conn_msgs[conn_id];
	auto pbuf = pstream->pread_buf;
	int msglen=0;
	if(!pmsg && pbuf->readable_size()>=sizeof(int))
	{
		msglen = pbuf->read_int();
		log_debug("try read msg:len:%d",msglen);
		pmsg = std::make_shared<Msg>(msglen);
		conn_msgs[conn_id] = pmsg; //save msg
	}
	if(pmsg && pbuf->readable_size()>=msglen)
	{
		pmsg->write(pbuf->read(msglen),msglen);
		handle_msg(conn_id,pmsg);
		conn_msgs.erase(conn_id);

	}
}
void MsgServer::handle_msg(int conn_id,ptmsg_t pmsg)
{
	log_debug("Msg server::handle_msg:%d,len:%d",
		conn_id,pmsg->len);
	//std::cout<<"msg raw date:"<<*(pmsg->get_data())<<std::endl;
	
	// ptmsg_t p = std::make_shared<msg
	//send_msg(conn_id,std::make_shared<Msg>(std::string("hello ghhhhhhhhhhhh")));
	//pingpong test
	std::string t = *(pmsg->get_data());
	t = t+t;
	ptmsg_t new_pmsg = std::make_shared<Msg>(t);
	send_msg(conn_id,new_pmsg);
}
void MsgServer::send_msg(int conn_id,ptmsg_t pmsg)
{
	log_debug("msg server send msg,id:%d,len:%d",
		conn_id,pmsg->len);
	pttcpstream_t pstream = streams[conn_id];
	assert(pstream);
	pstream->send(pmsg->get_raw());
}

void MsgServer::new_connect(int fd)
{
	TcpServer::new_connect(fd);
}



