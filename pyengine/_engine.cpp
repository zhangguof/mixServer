#include "pybase.hpp"
#include "services.hpp"
#include "eventloop.hpp"
#include "handles.hpp"

EventLoop* g_event_loop = NULL;

void _engine::init_methods()
{
	def("test",&test);
	def("send",&send);
	def("start_timer",&start_timer);
	def("regist_file_handle",&regist_file_handle);
}

// PyObject* _engine::test(PyObject* self,PyObject* args)
PYMOD_METHOD(_engine,test)
{
	printf("in _engine test!!\n");
	Py_RETURN_NONE;
}

// PyObject* _engine::send(PyObject* self,PyObject* args)
PYMOD_METHOD(_engine,send)
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

// PyObject* _engine::start_timer(PyObject* self,PyObject* args)
PYMOD_METHOD(_engine,start_timer)
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

class PyFileHandle:public Handle
{
public:
	bool init_file(const char* _file_path,PyObject* _cb)
	{
		file_path = _file_path;
		cb = _cb;
		return open();
	}
	void handle_read(){}
	void handle_write(){}
	void handle_error(){}
	void close(){}
	void handle_modify()
	{
		log_debug("handle modify:%s",file_path.c_str());
		PyObject* args = Py_BuildValue("(s#)",
			file_path.c_str(),file_path.size());
		PyObject* r = PyObject_CallObject(cb,args);
		if(!r)
		{
			log_err("call py err!");
			check_py_error();
		}
		Py_XDECREF(r);
		Py_XDECREF(args);

	}
	bool open()
	{
		//O_EVTONLY O_RDWR
		//O_DIRECTORY
		int _fd = ::open(file_path.c_str(),O_EVTONLY);
		// int _fd = ::opendir(file_path.c_str());
		if(_fd==-1)
			log_err("open file err:filepath:%s:%d:%s",file_path.c_str(),errno,get_error_msg(errno));
		assert(_fd!=-1);
		init(_fd,MODIFY);
		log_debug("open file success!fd:%d",_fd);
		return true;
		
	}
	PyObject* cb;
	std::string file_path;
};

PYMOD_METHOD(_engine,regist_file_handle)
{
	char* s;
	int size;
	PyObject* cb;
	int r = PyArg_ParseTuple(args,"s#O",&s,&size,&cb);
	if(r && cb && PyCallable_Check(cb))
	{
		log_debug("handle _engine:regist_file_handle!");
		auto phandle = std::make_shared<PyFileHandle>();
		if(!phandle->init_file(s,cb))
		{
			log_err("init file err!");
			PyErr_SetString(PyExc_RuntimeError,"_engine:regist_file_handle:init file err!");
			return NULL;
		}
		// phandle->init()
		g_event_loop->regist_handle(phandle);

	}
	else
	{
		log_err("_engine:regist_file_handle:args err!");
		PyErr_SetString(PyExc_RuntimeError,"_engine:regist_file_handle!!args err!");
	}
	Py_RETURN_NONE;
}



