#ifndef _SERVICES_H_
#define _SERVICES_H_
#include <memory>
#include "proto.hpp"
// #include "server.hpp"
class MsgServer;
class Test:public std::enable_shared_from_this<Test>
{
public:
	typedef std::shared_ptr<Test> ptthis_t;
	const static int service_id;
	Test(Proto* p,MsgServer* _pserver):
		proto(p),pserver(_pserver){
			//init();
		}
	Test(Proto* p):proto(p){
		//init();
	}
	void init();

	void on_echo(std::unique_ptr<Echo> p);
	std::shared_ptr<ProtoMsg> send_echo(const std::string& msg);
	
private:
	ptthis_t get_this()
	{
		return shared_from_this();
	}
	Proto* proto;
	MsgServer* pserver;
};
#endif