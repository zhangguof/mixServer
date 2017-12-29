#ifndef _PROTO_H
#define _PROTO_H
#include <string>
#include <memory>
#include <functional>

#include "test.pb.h"
#include "utils.hpp"

class HandleBase;
class Msg;
class ProtoMsg;

typedef ::google::protobuf::Message Message;
typedef std::shared_ptr<Message> ptmessage_t;
typedef std::unique_ptr<Message> unipt_message_t;

typedef std::shared_ptr<HandleBase> ptHandleBase_t;
typedef std::shared_ptr<Msg> ptmsg_t;
typedef std::shared_ptr<ProtoMsg> ptpmsg_t;


// typedef std::function<void(unipt_message_t)> Function;

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
	void write(const Message& pbmsg)
	{
		pbmsg.SerializeToString(get_ptdata());
		reset_len();
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
	virtual void operator()(int s_id,int c_id,int uid,unipt_message_t p)=0;
	virtual unipt_message_t get_ptmessage(const ptpmsg_t& pmsg)=0;
	// virtual ~HandleBase(){}
};


template<typename T,typename MT>
class MsgHanle:public HandleBase
{
public:
	typedef std::shared_ptr<MT> ptMT_T;
	typedef std::unique_ptr<MT> uniptMT_T;
	typedef std::shared_ptr<T> ptT_t;
	typedef void (T::*memf_t)(int,uniptMT_T p);

	MsgHanle(ptT_t t,memf_t _cb):pt(t),cb(_cb){}

	void operator()(int s_id,int c_id,int uid,unipt_message_t p);
	unipt_message_t get_ptmessage(const ptpmsg_t& pmsg)
	{
		auto pt = std::unique_ptr<MT>(new MT());
		pt->ParseFromString(*(pmsg->get_data()));
		return std::move(pt);
	}
	memf_t cb;
	ptT_t pt; 
};

template<typename T,typename MT>
void MsgHanle<T,MT>::operator()(int s_id,int c_id,int uid,unipt_message_t p)
{
	std::unique_ptr<MT> p_mt(static_cast<MT*>(p.release()));

	return ((pt.get())->*cb)(uid,std::move(p_mt));
}

class Proto:public Singleton<Proto>
{
public:
	//regist<MT>(xx,xxx,xx,xx);
	template<typename MT,typename T,typename memf_t>
	void regist(int s_id, int c_id,std::shared_ptr<T> pt,memf_t t);

	void _regist(int s_id,int c_id,std::shared_ptr<HandleBase> cb);

	void on_msg(int uid,const ptmsg_t& pmsg);

	void dispatch_msg(int s_id,int c_id,int uid,const ptpmsg_t& pmsg);
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