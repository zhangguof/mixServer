#include "proto.hpp"
#include <map>
#include <utility>
#include <memory>
#include <cassert>


// class Message;
// typedef void (*OnMsgCallBack)(int,int,Message*);

void Proto::_regist(int s_id,int c_id,std::shared_ptr<HandleBase> cb)
{
	auto key = std::make_pair(s_id,c_id);
	auto it_cb = protos.find(key);
	assert(it_cb == protos.end());
	protos[key] = cb;
}

void Proto::on_msg(const ptmsg_t& pmsg)
{
	auto p = std::static_pointer_cast<ProtoMsg>(pmsg);
	p->read_ids();
	printf("Proto::om_msg::s_id:%d,c_id:%d\n",p->service_id,p->command_id);


	dispatch_msg(p->service_id,p->command_id,p);
}


void Proto::dispatch_msg(int s_id,int c_id,const ptpmsg_t& pmsg)
{
	auto key = std::make_pair(s_id,c_id);
	auto it_cb = protos.find(key);
	assert(it_cb!=protos.end());
	auto msg_handle = it_cb->second;
	auto p_ptmessage = msg_handle->get_ptmessage(pmsg);

	(*msg_handle)(s_id,c_id,std::move(p_ptmessage));
}

