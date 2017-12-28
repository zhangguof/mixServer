#include <cassert>
#include <string>
#include <memory>
#include "services.hpp"

const int Test::service_id = 1;
void Test::init()
{
	proto->regist<Echo>(service_id,1,get_this(),&Test::on_echo);

}

void Test::on_echo(std::shared_ptr<Echo> p)
{
	printf("on echo:%s\n",p->msg().c_str());
}

std::shared_ptr<ProtoMsg> Test::send_echo(const std::string& msg)
{
	Echo p;
	int c_id = 1;
	p.set_service_id(service_id);
	p.set_command_id(c_id);
	p.set_msg(msg);
	auto pm = std::make_shared<ProtoMsg>(service_id,c_id);
	p.SerializeToString(pm->get_ptdata());
	pm->reset_len();
	return pm;
	//pserver->send_msg(1,pm);
}