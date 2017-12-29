#include "server.hpp"
#include <signal.h>
#include <iostream>
#include <functional>

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
	int n = acceptor->listen(server_ip,server_port);
	if(n<0)
	{
		log_debug("TcpServer start error!!!");
		return;
	}
	
	ploop->do_loop();

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
typedef std::function<void(int,const ptmsg_t& )> Sender_t;

MsgServer::MsgServer():TcpServer()
{
	using namespace std::placeholders;
	Sender_t f = std::bind(&MsgServer::send_msg,this,_1,_2);
	Test::get_inst()->init(f);
}

void MsgServer::handle_read(pttcpstream_t pstream)
{
	log_debug("msg server handle_read!");
	int conn_id = pstream->connect_id;
	assert(streams.find(conn_id)!=streams.end());

	ptmsg_t pmsg;

	auto find_it = conn_msgs.find(conn_id);
	if(find_it==conn_msgs.end())
	{
		pmsg = std::make_shared<Msg>();
		conn_msgs[conn_id] = pmsg;
	}
	else
	{
		pmsg = find_it->second;
	}

	auto pbuf = pstream->pread_buf;
	// int remain_msg_len = pmsg->get_remain_len();
	log_debug("===msg_len:%d,%d",pmsg->len,pmsg->get_remain_len());
	if(pmsg->len==0  && pbuf->readable_size()>=sizeof(int))
	{
		int msg_len = pbuf->read_int();
		log_debug("try read msg:len:%d",msg_len);
		pmsg->try_write(msg_len);
	}
	int remain_len = pmsg->get_remain_len();
	int read_size = pbuf->readable_size();
	if(remain_len > 0 && read_size>0)
	{
		if(read_size>remain_len)
			read_size = remain_len;
		pmsg->write(pbuf->read(read_size),read_size);
		remain_len = pmsg->get_remain_len();
		log_debug("==msg write:%d,remain:%d",read_size,
			remain_len);
		if(remain_len==0)
		{
			handle_msg(conn_id,pmsg);
			pmsg->clear();
			//if has readable buffer
			if(pbuf->readable_size()>=sizeof(int))
				handle_read(pstream);
		}
	}
}
void MsgServer::handle_msg(int conn_id,const ptmsg_t& pmsg)
{
	log_debug("Msg server::handle_msg:%d,len:%d",
		conn_id,pmsg->len);	

	Proto::get_inst()->on_msg(conn_id,pmsg);
	
	//pingpong test
	// std::string t = *(pmsg->get_data());
	// t = t+t;
	// ptmsg_t new_pmsg = std::make_shared<Msg>(t);
	// send_msg(conn_id,new_pmsg);
}
void MsgServer::send_msg(int conn_id,const ptmsg_t& pmsg)
{
	log_debug("msg server send msg,id:%d,len:%d",
		conn_id,pmsg->len);
	assert(pmsg->len > 0);
	pttcpstream_t pstream = streams[conn_id];
	assert(pstream);
	pstream->send(pmsg->get_raw());
}

void MsgServer::new_connect(int fd)
{
	TcpServer::new_connect(fd);
}



