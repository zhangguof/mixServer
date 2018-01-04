#include "pybase.hpp"
#include "services.hpp"
#include "eventloop.hpp"

EventLoop* g_event_loop = NULL;

void _engine::init_methods()
{
	def("test",&test);
	def("send",&send);
	def("start_timer",&start_timer);
}

PyObject* _engine::test(PyObject* self,PyObject* args)
{
	printf("in _engine test!!\n");
	Py_RETURN_NONE;
}

PyObject* _engine::send(PyObject* self,PyObject* args)
{
	printf("in _sender_send!!!!!\n");
	int uid;
	int s_id;
	int c_id;
	int size;
	char* s;
	int r = PyArg_ParseTuple(args,"iiis#",
		&uid,&s_id,&c_id,&s,&size); 
	if(r)
	{
		printf("on _engine:_send:%d,s_id:%d,c_id:%d,len:%d\n",
			uid,s_id,c_id,size);
		assert(strlen(s)==size);
		auto pm = std::make_shared<ProtoMsg>(s_id,c_id);
		pm->write(s,size);
		Services::get_inst()->sender(uid,pm);
		return Py_BuildValue("i",size);

	}
	else
	{
		printf("sneder:send err!\n");
		PyErr_SetString(PyExc_RuntimeError,"_engine:send err!!");
		return NULL;
	}
	Py_RETURN_NONE;
}

class PyTimerCb
{
public:
	PyTimerCb(PyObject* _f):pfun(_f){Py_INCREF(_f);}
	void on_handle(std::shared_ptr<Timer> ptimer)
	{
		PyObject* pr =  PyObject_CallObject(pfun,NULL);
		if(!pr)
		{
			printf("call py obj err!\n");
			if(check_py_error())
			{
				Py_INCREF(pfun);
			}
		}
		Py_XDECREF(pr);
	}
	~PyTimerCb(){
		Py_XDECREF(pfun);
	}
	PyObject* pfun;
};

PyObject* _engine::start_timer(PyObject* self,PyObject* args)
{
	int t;
	PyObject* pyf;
	int t_id;
	int r = PyArg_ParseTuple(args,"iO",&t,&pyf); //borrow ref!
	//add ref pyf??
	if(r && pyf && PyCallable_Check(pyf))
	{
		printf("on _engine start_timer:%d\n",t);
		auto pobj = std::make_shared<PyTimerCb>(pyf);
		u32 timer_id = g_event_loop->start_timer((u32)t,pobj,&PyTimerCb::on_handle);
		return Py_BuildValue("i",timer_id);
	}
	else
	{
		printf("parse args error!\n");
		PyErr_SetString(PyExc_RuntimeError,"_engine_start_timer err!!");
		return NULL;
	}
	Py_RETURN_NONE;
}

