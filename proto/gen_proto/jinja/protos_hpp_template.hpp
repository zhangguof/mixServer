#ifndef _PROTOS_H
#define _PROTOS_H
//gen by gen_pb.py!!~
{% for head_file in head_files%}
#include "{{head_file}}"
{% endfor %}
#endif