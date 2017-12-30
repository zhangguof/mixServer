#ifndef _SERVICES_H_
#define _SERVICES_H_
#include <memory>
#include "proto.hpp"
#include <functional>
typedef std::function<void(int,const std::shared_ptr<ProtoMsg>&)> Sender;

class MsgServer;

class Services:public Singleton<Services>
{
public:
	void init(Sender s);
	Sender sender;
};

class Test:public Singleton<Test>
{
public:
	const static int service_id;
	void init_commands();

	void init();

	void on_echo(int uid,std::unique_ptr<proto::Echo> p);
	void send_echo(int uid,const std::string& msg);

	void on_add(int uid,std::unique_ptr<proto::Add> p);
	void on_addret(int uid,std::unique_ptr<proto::AddRet> p);
	void send_add(int uid,int a,int b);
	
	void send_msg(int uid,int sid,int cid,const Message& p);
	
private:
	MsgServer* pserver;
	//Sender sender;
};
#endif