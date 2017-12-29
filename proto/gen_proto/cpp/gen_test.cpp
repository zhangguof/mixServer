#include "services.hpp"
#include "log.hpp"
//gen by gen_pb.py!!~

const int Test::service_id = 1;
void Test::init_commands()
{
	
	Proto::get_inst()->regist<proto::AddRet>(1,1002,get_this(),&Test::on_addret);
	
	Proto::get_inst()->regist<proto::Add>(1,1001,get_this(),&Test::on_add);
	
	Proto::get_inst()->regist<proto::Echo>(1,1,get_this(),&Test::on_echo);
	
	log_debug("Service:Test:init!");
}
