#include "services.hpp"
#include "log.hpp"
//gen by gen_pb.py!!~
{% for mod in mods %}
const int {{mod.name}}::service_id = {{mod.service_id}};
void {{mod.name}}::init()
{
	{% for method in mod.methods %}
	proto->regist<proto::{{method.msg_name}}>({{mod.service_id}},{{method.command_id}},get_this(),&{{mod.name}}::{{method.name}});
	{% endfor %}
	log_debug("Service:{{mod.name}}:init!");
}
{% endfor %}
