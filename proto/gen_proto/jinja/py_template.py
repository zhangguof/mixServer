#-*- coding:utf-8 -*-
#gen by gen_pb.py!!~
import proto
{% for mod in mods%}import protos.{{mod.file_name}}_pb2 as {{mod.file_name}}_pb2
import services.{{mod.name}}
{% endfor %}
def init():{% for mod in mods %}{% for method in mod.methods %}{% if method.handle == "py" %}
	proto.regist({{mod.service_id}},{{method.command_id}},
		{{mod.file_name}}_pb2.{{method.msg_name}},
		services.{{mod.name}}.{{method.name}}){% endif %}{% endfor %}{% endfor %}
	pass
