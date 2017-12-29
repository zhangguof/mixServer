#include "services.hpp"
#include "log.hpp"
//gen by gen_pb.py!!~

const int Test::service_id = 1;
void Test::init()
{
	
	proto->regist<proto::AddRet>(1,1002,get_this(),&Test::on_addret);
	
	proto->regist<proto::Add>(1,1001,get_this(),&Test::on_add);
	
	proto->regist<proto::Echo>(1,1,get_this(),&Test::on_echo);
	
	log_debug("Service:Test:init!");
}
