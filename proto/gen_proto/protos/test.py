#-*- coding:utf-8 -*-
SERVICE_ID = 1

Echo = {
	"cmd_id":1,
	"mod":"Test",
	"func":"on_echo",
	"args":(
		(str,"msg"),
		)
}

Add = {
	"cmd_id":1001,
	"mod":"Test",
	"func":"on_add",
	"args":(
		(int,"add1"),
		(int,"add2"),
		)
}

AddRet = {
	"cmd_id":1002,
	"mod":"Test",
	"func":"on_addret",
	"args":(
		(int,"ret"),
		)
}