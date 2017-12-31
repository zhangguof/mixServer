#include "server.hpp"
#include "log.hpp"
#include <iostream>

// enum HandleState
// {
// 	END=0,
// 	HREADING,
// };

/*
http handle state:
state:heand_reading //wait for /r/n
reading->
*/

struct HttpHeader
{
	std::string method;
	std::string request_url;
	std::string proto_type;
	std::string host;
};

struct HttResponese
{
	// std::string 
};

class httpHandle
{
public:
	httpHandle(TcpServer::pttcpstream_t _pstream){
		// state=END;
		pstream = _pstream;
	}
	void on_line(const std::string& str);
	void on_end()
	{
		log_debug("read end!!!");
		// std::cout<<"method:"<<header.method<<":url:"<<header.request_url<<
		// ":proto:"<<header.proto_type<<":host:"<<header.host<<std::endl;
		
		// pstream->close();
	}
	// int state;
	TcpServer::pttcpstream_t pstream;
	HttpHeader header;

};

void httpHandle::on_line(const std::string& str)
{
	if(str.size()==0)
	{
		return on_end();
	}
	int find_idx = str.find(' ');
	std::string filed_name = std::string(str.begin(),str.begin()+find_idx);
	++find_idx;
	if(filed_name=="GET")
	{
		header.method = "GET";
		int find_next = str.find(' ',find_idx);
		header.request_url = std::string(str.begin()+find_idx,str.begin()+find_next);
		header.proto_type = std::string(str.begin()+find_next+1,str.end());
	}
	else if(filed_name == "Host:")
	{
		header.host = std::string(str.begin()+find_idx,str.end());
	}
	else
	{
		log_debug("unknow filed:%s",str.c_str());
	}
}

void HttpServer::handle_read(pttcpstream_t pstream)
{
	log_debug("http handle read!");
	auto pbuf = pstream->pread_buf;
	int idx = pbuf->find_CRLF();
	while(1)
	{
		if(idx<0)
			break;
		char* p = pbuf->read(idx+2);
		// printf("read n:%d\n",idx);
		std::string line = "";
		if(idx>0)
			line.append(p,idx);
		on_read_line(pstream->connect_id,line);
		idx = pbuf->find_CRLF();
	}


}

void HttpServer::new_connect(int fd)
{
	TcpServer::new_connect(fd);

}

void HttpServer::close_connect(pttcpstream_t pstream)
{
	log_debug("http:on close_connect!!");
	TcpServer::close_connect(pstream);
	auto it = handles.find(pstream->connect_id);
	assert(it!=handles.end());
	handles.erase(it);
}

void HttpServer::handle_request(){

}
void HttpServer::on_read_line(int c_id,const std::string& str)
{
	auto it_handle = handles.find(c_id);
	if(it_handle == handles.end())
	{
		handles[c_id] = std::make_shared<httpHandle>(streams[c_id]);
	}
	auto phandle = handles[c_id];
	phandle->on_line(str);

	// log_debug("read line:%s",str.c_str());
}
void HttpServer::on_get(){

}
void HttpServer::on_post(){

}