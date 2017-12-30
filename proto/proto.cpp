#include "proto.hpp"
#include <map>
#include <utility>
#include <memory>
#include <cassert>

//python handle msg!
void handle_pb_msg(int s_id,int c_id,int uid,const char* s);

void Proto::_regist(int s_id,int c_id,std::shared_ptr<HandleBase> cb)
{
	auto key = std::make_pair(s_id,c_id);
	auto it_cb = protos.find(key);
	assert(it_cb == protos.end());
	protos[key] = cb;
}

void Proto::on_msg(int uid,const ptmsg_t& pmsg)
{
	auto p = std::static_pointer_cast<ProtoMsg>(pmsg);
	p->read_ids();
	printf("Proto::om_msg::s_id:%d,c_id:%d\n",p->service_id,p->command_id);

	handle_pb_msg(p->service_id,p->command_id,uid,p->get_data()->c_str());

	//dispatch_msg(p->service_id,p->command_id,uid,p);
}


void Proto::dispatch_msg(int s_id,int c_id,int uid,const ptpmsg_t& pmsg)
{
	auto key = std::make_pair(s_id,c_id);
	auto it_cb = protos.find(key);
	assert(it_cb!=protos.end());
	auto msg_handle = it_cb->second;
	auto p_ptmessage = msg_handle->get_ptmessage(pmsg);

	(*msg_handle)(s_id,c_id,uid,std::move(p_ptmessage));
}

