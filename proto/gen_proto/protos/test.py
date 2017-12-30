#-*- coding:utf-8 -*-
from types import *
SERVICE_ID = 1000

Echo = {
	"cmd_id":1,
	"side":SERVER,
	"handle":PY,
	"mod":"Test",
	"func":"on_echo",
	"args":(
		(str,"msg"),
		)
}

C_Echo = {
	"cmd_id":2,
	"side":CLIENT,
	"handle":CPP,
	"mod":"Test",
	"func":"on_echo",
	"args":(
		(str,"msg"),
		)
}


Add = {
	"cmd_id":1001,
	"side":SERVER,
	"handle":PY,
	"mod":"Test",
	"func":"on_add",
	"args":(
		(int,"add1"),
		(int,"add2"),
		)
}

AddRet = {
	"cmd_id":1002,
	"side":CLIENT|SERVER,
	"handle":CPP,
	"mod":"Test",
	"func":"on_addret",
	"args":(
		(int,"ret"),
		)
}