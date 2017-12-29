#ifndef _SERVICES_H_
#define _SERVICES_H_
#include <memory>
#include "proto.hpp"
#include <functional>
// #include "server.hpp"
typedef std::function<void(int,const std::shared_ptr<ProtoMsg>&)> Sender;
class MsgServer;

class Test:public std::enable_shared_from_this<Test>
{
public:
	typedef std::shared_ptr<Test> ptthis_t;
	const static int service_id;
	Test(Proto* p,MsgServer* _pserver):
		proto(p),pserver(_pserver){}
	Test(Proto* p):proto(p){}
	Test(Proto*p,Sender s):proto(p),sender(s){}
	void init();

	void on_echo(int uid,std::unique_ptr<proto::Echo> p);
	void send_echo(const std::string& msg);

	void on_add(int uid,std::unique_ptr<proto::Add> p);
	void on_addret(int uid,std::unique_ptr<proto::AddRet> p);
	void send_add(int uid,int a,int b);
	void send_msg(int uid,int sid,int cid,const Message& p);
	
private:
	ptthis_t get_this()
	{
		return shared_from_this();
	}
	Proto* proto;
	MsgServer* pserver;
	Sender sender;
};
#endif