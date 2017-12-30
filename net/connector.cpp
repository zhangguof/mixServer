
#include "connector.hpp"
#include "services.hpp"
#include <functional>

Connector::Connector(std::weak_ptr<Client> _client)
{
	psocket = std::make_shared<Socket>();
	pclient = _client;
	status = CLOSED;
	pwrite_buf = std::make_shared<Buffer>();
	pread_buf = std::make_shared<Buffer>();


	init(psocket->socket_fd,WRITE|READ); //only write, use for check error.
}
int Connector::connect(std::string _ip,port_t _port)
{
	assert(status == CLOSED);
	assert(psocket);
	ip 	 = _ip;
	port = _port;

	return _connect();
}

int Connector::_connect()
{
	int n = psocket->connect(ip,port);
	if(n>0){
		status = CONNECTED;
		return 0;
	}
	if(n<0 && (errno==EALREADY || errno==EINPROGRESS))
	{
		status = CONNECTING;
		return 0;
	}
	log_debug("connect error:%d,%s",errno,get_error_msg(errno));
	return n;
}

void Connector::handle_read()
{
	assert(status == CONNECTED || status==CONNECTING);
	log_debug("Connector handle read!!");
	if(status == CONNECTING)
	{
		int n;
		n = psocket->recv(0);
		if(n<0)
		{
			log_debug("read try connect error:%d,%s,%s",
				errno,get_error_msg(errno),strerror(errno)
				);
			close();
			return;
		}
	}
	else if(status == CONNECTED)
	{
		int n = psocket->recv(pread_buf);
		if(n==0 || (n<0 && errno!=EAGAIN && errno!=EWOULDBLOCK))
		{
			if(n<0)
				log_debug("error,no:%d,msg:%s:%s",
					errno,get_error_msg(errno),strerror(errno));
			else
				log_debug("peer close!!!");
			// log_debug("test:%d,%d,%s",n,errno,get_error_msg(errno));
			close();
			return;
		}
		get_client()->on_read();
	}

}
void Connector::handle_write()
{

 // After select(2) indicates writability, use getsockopt(2) to read the
 //              SO_ERROR option at level SOL_SOCKET to determine whether
 //              connect() completed successfully (SO_ERROR is zero) or
 //              unsuccessfully (SO_ERROR is one of the usual error codes
 //              listed here, explaining the reason for the  failure).
	assert(status==CONNECTED||status==CONNECTING);
	if(status==CONNECTING)
	{
		log_debug("check connecting in select....");
		int err = -1;
		unsigned int len = sizeof(err);
		psocket->get_opt(SOL_SOCKET,SO_ERROR,&err,&len);
		if(err == 0)
		{
			log_debug("Connector connected success!!");
			status = CONNECTED;
			disable_write();
			//enable_read();
			get_client()->on_connected();
		}
		else
		{
			log_debug("connect error!!%d:%d,%s",errno,err,get_error_msg(err));
			close();
		}

	}
	else
	{
		// do send here.....
		log_debug("Conector handle handle_write!");
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
}

void Connector::send(std::string s)
{
	pwrite_buf->write(s);
	enable_write();
}
void Connector::send(const char* p,int size)
{
	pwrite_buf->write(p,size);
	enable_write();
}
void Connector::send(std::shared_ptr<std::string> ps)
{
	send(*ps);
}
void Connector::handle_error(){}
void Connector::close()
{
	//psocket->close();
	status = CLOSED;
	get_client()->on_close();
}

Client::Client()
{
	using namespace std::placeholders;

	ploop = std::make_shared<EventLoop>();
	msg_reading = false;
	pmsg = std::make_shared<Msg>();
	retry_count = 0;
	status = CLOSED;

	// proto = std::make_shared<Proto>();
	Sender f = std::bind((send_memf)&Client::send_msg,this,_2);
	// ps_test = std::make_shared<Test>(proto.get(),f);
	// ps_test->init();
	Test::get_inst()->init();
	Services::get_inst()->init(f);
	log_debug("after make Client");
}
Client::~Client()
{
	log_debug("release client....:loop:%d",ploop.use_count());
}

int Client::start_connect(std::string _ip, 
	port_t _port)
{
	ip = _ip;
	port = _port;
	retry_count = 0;
	int n = _start_connect();
	do_timeout_check();
	return n;
}

int Client::_start_connect()
{
	assert(status==CLOSED);
	pconn = std::make_shared<Connector>(get_this());
	pconn->set_loop(ploop);
	int n = pconn->connect(ip,port);
	if(n<0)
	{
		log_debug("client connect err!!");
		// log_debug("error:%d,%d,%s",n,errno,get_error_msg(errno));
		return n;
	}
	ploop->regist_handle(pconn);
	status = CONNECTING;
	++retry_count;
	return 1;
}

void Client::do_timeout_check()
{
	log_debug("first regist timeout check!!");
	ploop->start_timer(retry_timeout,get_this(),
		&Client::_re_connect);
}
void Client::_re_connect(Timer::pttimer_t ptimer)
{
	assert(pconn);
	log_debug("in _re_connect!!:%d",retry_count);
	if(retry_count > max_retry_cout)
	{
		log_debug("re_connect max times!!");
		return;
	}

	if(status == CONNECTED)
		return;
	if(status == CONNECTING)
	{
		pconn->close();

	}
	if(status == CLOSED)
	{
		_start_connect();
	}
	ptimer->start_timer(retry_timeout,get_this(),
		&Client::_re_connect);
}



void Client::on_connected()
{
	auto addr = pconn->get_addr();
	log_debug("Client connected success!!!%s,%d",addr.ip.c_str(),addr.port);
	retry_count = 0;
	status = CONNECTED;
	std::string s = "hell world!!";
	Test::get_inst()->send_echo(0,s);
	Test::get_inst()->send_add(0,10,20);
}


void Client::on_read()
{
	log_debug("client handle read...");
	// assert(pconn);
	auto pbuf = pconn->pread_buf;

	assert(pmsg);
	log_debug("===test:%d,%d",pmsg?1:0,pmsg->len);

	if(pmsg->len==0 && pbuf->readable_size()>=sizeof(int))
	{
		int msg_len = pbuf->read_int();
		log_debug("try read msg:len:%d",msg_len);
		pmsg->try_write(msg_len);
	}
	int remain_len = pmsg->get_remain_len();
	int read_size = pbuf->readable_size();
	if(remain_len>0 && read_size>0)
	{
		if(read_size > remain_len)
			read_size = remain_len;
		pmsg->write(pbuf->read(read_size),read_size);
		remain_len = pmsg->get_remain_len();
		log_debug("===msg write:%d,remain:%d",read_size,remain_len);
		if(remain_len == 0)
		{
			on_msg(pmsg);
			pmsg->clear();
			if(pbuf->readable_size()>=sizeof(int))
				on_read();
		}
	}
}
void Client::on_msg(ptmsg_t pmsg)
{
	log_debug("client:on_msg:len:%d",
		pmsg->len);
	assert(pmsg->len > 0);
	Proto::get_inst()->on_msg(0,pmsg);
	//std::cout<<"msg raw data:"<<*(pmsg->get_data())<<std::endl;

//pingpong test
	// std::string t = *(pmsg->get_data());
	// t = t+t;
	// ptmsg_t new_pmsg = std::make_shared<Msg>(t);
	// send_msg(new_pmsg);
}
void Client::send_msg(const ptmsg_t& pmsg)
{
	pconn->send(pmsg->get_raw());
}
void Client::send_msg(const char* p,int size)
{
	ptmsg_t pmsg = std::make_shared<Msg>(p,size);
	send_msg(pmsg);
}

void Client::on_close()
{
	log_debug("close connect!!");
	ploop->unregist_handle(pconn);
	status = CLOSED;
	if(retry_count > max_retry_cout)
	{
		log_debug("max retry_count!!shutdown");
		ploop->shutdown();
	}
	else
	{
		//re_connect();
	}
}






