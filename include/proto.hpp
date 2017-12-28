#ifndef _PROTO_H
#define _PROTO_H
#include <string>
#include <memory>

#include "proto/test.pb.h"
// #include <google/protobuf/message.h>

class HandleBase;
class Msg;
class ProtoMsg;

typedef ::google::protobuf::Message Message;
typedef std::shared_ptr<Message> ptmessage_t;

typedef std::shared_ptr<HandleBase> ptHandleBase_t;
typedef std::shared_ptr<Msg> ptmsg_t;
typedef std::shared_ptr<ProtoMsg> ptpmsg_t;

template<typename T>
void pack_int(char*p,T a)
{
	int size = sizeof(T);
	// T* pa = &a;
	int aa = a;
	char* src = reinterpret_cast<char*>(&aa);
	std::copy(src,src+size,p);
}

class Msg
{
public:
	typedef std::shared_ptr<std::string> ptsting_t;
	typedef std::shared_ptr<Msg> ptmsg_t;
	Msg();
	Msg(int _len);
	Msg(const std::string& s);
	Msg(const char*p, int size);
	void init_str()
	{
		praw_data = std::make_shared<std::string>();
	}
	void clear();
	void try_write(int _len);
	void read(const char*p, int size);
	void write(const char*p, int size);
	void write(const std::string& s);
	
	virtual std::string get_raw()
	{
		char buf[sizeof(int)];
		pack_int(buf,len);
		return std::string(buf,sizeof(int)) + *praw_data;
	}
	ptsting_t get_data()
	{
		return praw_data;
	}
	std::string* get_ptdata()
	{
		return praw_data.get();
	}
	void reset_len()
	{
		len = praw_data->size();
	}
	int get_remain_len()
	{
		return len - has_write_size;
	}
	int len;
	bool msg_reading;
	ptsting_t praw_data;
private:
	int has_write_size;
};

class ProtoMsg:public Msg
{
public:
	ProtoMsg(int s_id,int c_id){
		service_id = s_id;
		command_id = c_id;

	}
	std::string get_raw()
	{
		char buf[3*sizeof(int)];
		char*p = buf;
		
		pack_int(p,len+2*sizeof(int));
		p+= sizeof(int);
		
		pack_int(p,service_id);
		p+=sizeof(int);
		
		pack_int(p,command_id);

		return std::string(buf,sizeof(buf)) + *praw_data;
	}
	void set_id(int s_id,int c_id)
	{
		service_id = s_id;
		command_id = c_id;
	}
	void read_ids()
	{
		//char buf[2*sizeof(int)];
		assert(praw_data->size()>2*sizeof(int));
		const char *pbuf = praw_data->data();
		service_id = *(reinterpret_cast<const int*>(pbuf));
		command_id = *(reinterpret_cast<const int*>(pbuf+sizeof(int)));
		praw_data->erase(0,2*sizeof(int));
	}

	int service_id;
	int command_id;
	// ptmessage_t p_pbmsg;
private:


};



class HandleBase
{
public:
	// HandleBase(){}
	// virtual void operator()(int s_id,int c_id,ptmessage_t p)=0;
	virtual void handle(int s_id,int c_id,ptmessage_t p)=0;
	virtual ptmessage_t get_ptmessage(const ptpmsg_t& pmsg)=0;
	// virtual ~HandleBase(){}
};


template<typename T,typename MT>
class MsgHanle:public HandleBase
{
public:
	typedef std::shared_ptr<MT> ptMT_T;
	typedef std::shared_ptr<T> ptT_t;
	typedef void (T::*memf_t)(ptMT_T p);

	MsgHanle(ptT_t t,memf_t _cb):pt(t),cb(_cb){}

	void handle(int s_id,int c_id,ptmessage_t p);
	ptmessage_t get_ptmessage(const ptpmsg_t& pmsg)
	{
		auto pt = std::make_shared<MT>();
		pt->ParseFromString(*(pmsg->get_data()));
		return pt;
	}
	memf_t cb;
	ptT_t pt; 
};

template<typename T,typename MT>
void MsgHanle<T,MT>::handle(int s_id,int c_id,ptmessage_t p)
{

	return ((pt.get())->*cb)(std::static_pointer_cast<MT>(p));
}

class Proto
{
public:
	//regist<MT>(xx,xxx,xx,xx);

	template<typename MT,typename T,typename memf_t>
	void regist(int s_id, int c_id,std::shared_ptr<T> pt,memf_t t);

	void _regist(int s_id,int c_id,std::shared_ptr<HandleBase> cb);

	void on_msg(const ptmsg_t& pmsg);

	void dispatch_msg(int s_id,int c_id,const ptpmsg_t& pmsg);
public:
	std::map<std::pair<int,int>,ptHandleBase_t > protos;
};


template<typename MT,typename T,typename memf_t>
void Proto::regist(int s_id, int c_id,std::shared_ptr<T> pt,memf_t t)
{
	auto pmsg = std::make_shared<MsgHanle<T,MT> >(pt,t);
	_regist(s_id,c_id,pmsg);
}




#endif