#include "services.hpp"
#include "log.hpp"
//gen by gen_pb.py!!~

const int Test::service_id = 1000;
void Test::init_commands()
{
	
	Proto::get_inst()->regist<proto::AddRet>(1000,1002,get_this(),&Test::on_addret);
	
	log_debug("Service:Test:init!");
}
