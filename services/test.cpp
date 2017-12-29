#include <cassert>
#include <string>
#include <memory>
#include "services.hpp"
#include "log.hpp"

void Test::init(Sender s)
{
	sender = s;
	init_commands();
}

void Test::on_echo(int _id,std::unique_ptr<proto::Echo> p)
{
	printf("on echo:%s\n",p->msg().c_str());
	if(_id>0)
	{
		//in server part;
		send_echo(_id,"echo:"+p->msg());
	}
}

void Test::on_add(int uid,std::unique_ptr<proto::Add> p)
{
	int a = p->add1();
	int b = p->add2();
	int s = a+b;
	proto::AddRet pr;
	pr.set_ret(s);
	send_msg(uid,pr.service_id(),pr.command_id(),pr);
}
void Test::on_addret(int uid,std::unique_ptr<proto::AddRet> p)
{
	printf("Test:addret:uid:%d,%d\n",uid,p->ret());
}

void Test::send_add(int uid,int a,int b)
{
	proto::Add p;
	p.set_add1(a);
	p.set_add2(b);
	send_msg(uid,p.service_id(),p.command_id(),p);
}

std::shared_ptr<ProtoMsg> make_proto_msg(int s_id,int c_id,const Message& p)
{
	auto pm = std::make_shared<ProtoMsg>(s_id,c_id);
	pm->write(p);
	return pm;
}

void Test::send_echo(int uid,const std::string& msg)
{
	proto::Echo p;
	p.set_msg(msg);
	send_msg(uid,p.service_id(),p.command_id(),p);

}

void Test::send_msg(int uid,int sid,int cid,const Message& p)
{
	sender(uid,make_proto_msg(sid,cid,p));
}
