
#include "connector.hpp"

Connector::Connector(std::weak_ptr<Client> _client)
{
	psocket = std::make_shared<Socket>();
	pclient = _client;
	status = CLOSED;
	pwrite_buf = std::make_shared<Buffer>();
	pread_buf = std::make_shared<Buffer>();

	init(psocket->socket_fd,WRITE); //only write, use for check error.
}
void Connector::connect(std::string ip,unsigned short port)
{
	assert(status == CLOSED);
	assert(psocket);
	int n = psocket->connect(ip,port);
	if(n>0){
		status = CONNECTED;
		return;
	}
	if(n<0 && (errno==EALREADY || errno==EINPROGRESS))
	{
		status = CONNECTING;
		return;
	}
	log_debug("connect error:%d,%d,%s",n,errno,get_error_msg(errno));

}

void Connector::handle_read()
{
	log_debug("Connector handle read!!");
	int n = psocket->recv(pread_buf);
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
	get_client()->on_read();
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
			enable_read();
			get_client()->on_connected();
		}
		else
		{
			log_debug("connect error!!%d:%d,%s",errno,err,get_error_msg(err));
		}

	}
	else
	{
		log_debug("Conector handle handle_write!");
		assert(pwrite_buf);
	}
}
void Connector::handle_error(){}
void Connector::close()
{
	psocket->close();
	get_client()->on_close();
}


void Client::start_connect(std::string ip, unsigned short port)
{
	pconn = std::make_shared<Connector>(get_this());
	pconn->connect(ip,port);
	ploop->regist_handle(pconn);
}

void Client::on_connected()
{
	auto addr = pconn->get_addr();
	log_debug("Client connected success!!!%s,%d",addr.ip.c_str(),addr.port);
	//pconn->close();
}

void Client::on_read()
{
	log_debug("client handle read...");
	int size = pconn->pread_buf->readable_size();
	char* pbuf = pconn->pread_buf->read();
	pbuf[size] = '\0';
	printf("read str:%s,%d\n",pbuf,size);
	//once read close it.
	// pconn->close();
}
void Client::on_close()
{
	log_debug("close connect!!");
	ploop->unregist_handle(pconn);
	ploop->shutdown();

}






