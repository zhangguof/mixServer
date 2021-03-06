#include "server.hpp"
#include "log.hpp"
#include <iostream>
#include <vector>
#include "Python.h"
#include "pyobjs.hpp"

typedef std::function<void(const char*, int)> send_raw_t;
//py interface
extern void py_new_connect(int conn_id,PyObject* sender);
extern void py_handle_line(int conn_id,const char* s,int size);
extern void py_handle_message(int conn_id, const char* s,int size);
extern void py_handle_close(int conn_id);


extern pySender* new_pysender(ptstream_t ps);
//py interface end

void string_split(char ch,const std::string& str,std::vector<std::string>& v)
{
	int find_pos = str.find(ch);
	int start_pos = 0;
	std::string s;
	while(find_pos!=std::string::npos)
	{
		if(find_pos>start_pos)
		{
			s = std::string(str,start_pos,find_pos-start_pos);
			v.push_back(s);
		}
		start_pos = find_pos+1;
		find_pos = str.find(ch,start_pos);
	}
	if(str.size()-start_pos>0)
	{
		s = std::string(str,start_pos,str.size()-start_pos);
		v.push_back(s);	
	}

}

struct HttpHeader
{
	std::string method;
	std::string req_url;
	std::string proto_type;
	std::map<std::string,std::string> _map;
	void set(std::string filed_name, const std::string& v)
	{
		_map[filed_name] = v;
	}
	const std::string& get(const std::string& name)
	{
		return _map[name];
	}
};


struct HttpResponese
{
	std::string status_code; //HTTP/1.1 200 OK
	std::map<std::string,std::string> _map;
	void set(std::string field_name, const std::string& v)
	{
		_map[field_name] = v;
	}
	const std::string& get(const std::string& name)
	{
		return _map[name];
	}
	void pack_str(std::string& s)
	{
		// std::string s;
		s.append("HTTP/1.1 "+status_code+"\r\n");
		for(auto it:_map)
		{
			s.append(it.first+": "+it.second+"\r\n");
		}
		s.append("\r\n");
		// return s;
	}
};

class httpHandle
{
public:
	httpHandle(TcpServer::pttcpstream_t _pstream){
		// state=END;

		pstream = _pstream;
		conn_id = pstream->connect_id;
		log_debug("new httpHandle!!:%d",conn_id);

		// py handle new connect!
		pySender* _p_sender = new_pysender(_pstream);
		py_new_connect(conn_id,(PyObject*)_p_sender);
		p_sender = _p_sender;
		// Py_DECREF(p_sender);
	}
	void handle_read();
	void on_line(const std::string& str);
	void on_end()
	{
		log_debug("read end!!!");
		std::cout<<"method:"<<header.method<<":url:"<<header.req_url<<
		":proto:"<<header.proto_type<<std::endl;
		for(auto it:header._map)
		{
			std::cout<<it.first<<":"<<it.second<<std::endl;;
		}
		// pstream->close();
	}
	void on_request()
	{
		HttpResponese resp;
		std::string html_code = "<html>"
		"<head>"
		"<title>An Example Page</title>"
		"</head>"
		"<body>"
		"Hello World, this is a very simple HTML document."
		"</body>"
		"</html>";
		resp.status_code = "200 OK";
		resp.set("Content-Encoding","utf-8");
		resp.set("Content-Length",std::to_string(html_code.size()));
		resp.set("Content-Type","text/html; charset=UTF-8");
		std::string reps_str;
		resp.pack_str(reps_str);
		pstream->send(reps_str+html_code);
		// pstream->close();
		if(header.get("Connection") == "keep-alive")
		{
			log_debug("Connection:keep-alive,don't close it!");
		}
		else
		{
			log_debug("Connection:keep-alive test:close it!%s",header.get("Connection").c_str());
			pstream->close();
		}

	}
	void on_close()
	{
		py_handle_close(conn_id);
		Py_XDECREF(p_sender);
	}
	// int state;
	TcpServer::pttcpstream_t pstream;
	HttpHeader header;
	int conn_id;
	~httpHandle()
	{
		printf("release httpHandle !!!\n");
	}
	// bool reading_line;
	pySender* p_sender;


};

void httpHandle::handle_read()
{
	auto pbuf = pstream->pread_buf;
	int c_id  = pstream->connect_id;
	if(!(p_sender->reading_line))
	{
		int n 	= pbuf->readable_size();
		assert(n>0);
		char* p = pbuf->read();
		py_handle_message(c_id,p,n);
		return;
	}

	int idx = pbuf->find_CRLF();
	while(1)
	{
		if(idx<0)
			break;
		char* p = pbuf->read(idx+2);
		// printf("read n:%d\n",idx);
		// std::string line = "";
		// if(idx>0)
		// 	line.append(p,idx);
		// on_read_line(pstream->connect_id,line);
		// on_line(line);
		py_handle_line(c_id,p,idx);
		idx = pbuf->find_CRLF();
	}
}

void httpHandle::on_line(const std::string& str)
{
	//python do handle
	// py_handle_line(conn_id,str.c_str(),str.size());
	// if(str.size()==0)
	// {
	// 	return on_request();
	// }
	// auto find_it = str.find(": ");
	// if(find_it == std::string::npos)
	// {
	// 	std::vector<std::string> v;
	// 	string_split(' ',str,v);
	// 	std::string filed_name = v[0];
	// 	assert(filed_name == "GET" || filed_name == "POSH");
	// 	assert(v.size()==3);

	// 	header.method = filed_name;
	// 	header.req_url = v[1];
	// 	header.proto_type = v[2];

	// }
	// else
	// {
	// 	std::string filed_name = std::string(str.begin(),str.begin()+find_it);
	// 	std::string filed_val = std::string(str.begin()+find_it+2,str.end());
	// 	header.set(filed_name,filed_val);
	// }
}

void HttpServer::handle_read(pttcpstream_t pstream)
{
	int c_id = pstream->connect_id;
	log_debug("http handle read!:%d",c_id);
	using namespace std::placeholders;
	assert(streams.find(c_id)!=streams.end());

	auto it_handle = handles.find(c_id);
	if(it_handle == handles.end())
	{
		handles[c_id] = std::make_shared<httpHandle>(pstream);
	}
	auto phandle = handles[c_id];
	phandle->handle_read();
	// auto pbuf = pstream->pread_buf;
	// int idx = pbuf->find_CRLF();
	// while(1)
	// {
	// 	if(idx<0)
	// 		break;
	// 	char* p = pbuf->read(idx+2);
	// 	// printf("read n:%d\n",idx);
	// 	std::string line = "";
	// 	if(idx>0)
	// 		line.append(p,idx);
	// 	on_read_line(pstream->connect_id,line);
	// 	idx = pbuf->find_CRLF();
	// }

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
	it->second->on_close();
	handles.erase(it);
}

void HttpServer::handle_request(){

}
// void HttpServer::on_read_line(int c_id,const std::string& str)
// {
// 	using namespace std::placeholders;
// 	log_debug("on_read_line:%d",c_id);
// 	assert(streams.find(c_id)!=streams.end());
// 	auto it_handle = handles.find(c_id);
// 	if(it_handle == handles.end())
// 	{

// 		handles[c_id] = std::make_shared<httpHandle>(streams[c_id]);
// 	}
// 	auto phandle = handles[c_id];

// 	phandle->on_line(str);

// 	log_debug("read line:%s",str.c_str());
// }
void HttpServer::on_get(){

}
void HttpServer::on_post(){

}