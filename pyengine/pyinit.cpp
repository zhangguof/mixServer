#include "Python.h"
#include <memory>
#include <stdarg.h>
#include "stdio.h"

#include "proto.hpp"
#include "services.hpp"
#include "timer.hpp"
#include "eventloop.hpp"
#include "pybase.hpp"



const char* py_str = "import sys\n"\
					 "sys.path.insert(0,'./script')\n"\
					 "import init\n"\
					 "init.init()\n";
char* pyhome = "./script";

PyObject* call_py_obj(const char* ,
	const char* ,
	const char* ,...);

void init_engine();
int g_py_init = 0;
EventLoop* g_event_loop = NULL;

extern"C" {
	extern void init_message(void);
	extern void init_api_implementation(void);
}

void init_pb_mod()
{
	//like _message
	init_message();
	init_api_implementation();
}

void init_python(int argc,char** argv)
{
	Py_NoSiteFlag = 1;
	Py_SetProgramName(argv[0]);
	Py_SetPythonHome(pyhome);

	Py_Initialize();
	PySys_SetArgvEx(argc, argv, 0);
	

	init_engine();

}

void check_py_error()
{
	PyObject* p = PyErr_Occurred();
	if(p)
	{
		PyErr_Print();
	}
}


void run_py()
{
	/* Execute some Python statements (in module __main__) */
    //PyRun_SimpleString(py_str);

    PyObject* sys_mod = PyImport_ImportModule("sys");
    PyObject* sys_path = PyObject_GetAttrString(sys_mod,"path");
    PyObject* str = PyString_FromString("./script");

    int ret = PyList_Insert(sys_path,0,str);
    if(ret==-1)
    	printf("insert error!\n");
    PyObject* pmod  = PyImport_ImportModule("init");
    if(!pmod)
    {
    	check_py_error();
    	return;
    }
    PyObject* f_init = PyObject_GetAttrString(pmod,"init");
    if(f_init && PyCallable_Check(f_init))
    {
    	PyObject* pret = PyObject_CallObject(f_init,NULL);
    	if(pret)
    	{
    		printf("init success!!\n");
    	}
    	Py_XDECREF(pret);

    }
    else
    {
    	printf("init error!!\n");
    }
    pyTest_* obj = class_<pyTest_>::new_obj(NULL,NULL);
    // obj->xxx = 1024;
    printf("ojb name:%s,cnt:%d\n",obj->ob_type->tp_name,obj->ob_refcnt);
    call_py_obj("init","test","(O)",obj);
    printf("after call name:%s,cnt:%d\n",obj->ob_type->tp_name,obj->ob_refcnt);
    
    Py_XDECREF(obj);

    // printf("after call:%d\n",obj->xxx);
    // delete obj;

    Py_XDECREF(f_init);
    Py_XDECREF(pmod);
    Py_XDECREF(str);
    Py_XDECREF(sys_path);
    Py_XDECREF(sys_mod);

}

int init_py(int argc,char** argv)
{

	init_python(argc,argv);
	// printf("Hello !!!\n");
	run_py();
	init_pb_mod();

	//main.main()
	PyRun_SimpleString("import main\nmain.main()\n");
	// Py_Exit(0);
	g_py_init = 1;
	return 0;
}



PyObject* call_py_obj(const char* modname,
	const char* fun_name,
	const char* args_fmt,...)
{
	PyObject* pmod = PyImport_ImportModule(modname);
	if(!pmod)
	{
		printf("import mod err!\n");
		check_py_error();
		return NULL;
	}
	PyObject* pfun = PyObject_GetAttrString(pmod,fun_name);
	PyObject* args;
	PyObject* pr = NULL;

	//build  args
	va_list vargs;
	va_start( vargs, args_fmt );
	args = Py_VaBuildValue( args_fmt, vargs );
	va_end(vargs);

	if(args && pfun && PyCallable_Check(pfun))
	{
		pr =  PyObject_CallObject(pfun,args);
		if(!pr)
		{
			printf("call py obj err!\n");
			check_py_error();
		}
		Py_XDECREF(pr);
	}

	Py_XDECREF(args);
	Py_XDECREF(pfun);
	Py_XDECREF(pmod);
	return pr;
}

void on_update(int interval)
{
	if(!g_py_init)
		return;
	call_py_obj("main","update","(i)",interval);
}

int handle_pb_msg(int s_id,int c_id,int uid,const char* s)
{
	if(!g_py_init)
	{
		printf("py not init!!!!");
		return 0;
	}
	int ret = 0;
	//imoprt proto;
	//proto.HandleMsg(sid,cid,pbstr)
	PyObject* pr = call_py_obj("proto","HandleMsg","(iiis)",
							  s_id,c_id,uid,s);
	if(pr)
	{
		ret = PyInt_AsLong(pr);
	}


	
	if(!ret)
		printf("handle_msg error!:%d,%d\n",s_id,c_id);
	return ret;

}

static PyObject* _engine_send(PyObject* self,PyObject* args)
{
	printf("in _sender_send!!!!!\n");
	int uid;
	int s_id;
	int c_id;
	int size;
	char* s;
	//_send.send(uid,pb_str)
	int r = PyArg_ParseTuple(args,"iiis#",
		&uid,&s_id,&c_id,&s,&size); 
	if(r)
	{
		printf("on _sender:send:%d,s_id:%d,c_id:%d,len:%d\n",
			uid,s_id,c_id,size);
		assert(strlen(s)==size);
		auto pm = std::make_shared<ProtoMsg>(s_id,c_id);
		pm->write(s,size);
		Services::get_inst()->sender(uid,pm);
		
		// return Py_BuildValue("i",size);

	}
	else
	{
		printf("sneder:send err!\n");
		PyErr_SetString(PyExc_RuntimeError,"_sender:send err!!");
		return NULL;
	}
	Py_RETURN_NONE;
}

class PyTimerCb
{
public:
	PyTimerCb(PyObject* _f):pfun(_f){}
	void on_handle(std::shared_ptr<Timer> ptimer)
	{
		// PyObject* pargs = Py_BuildValue("(i)",timer_id);
		PyObject* pr =  PyObject_CallObject(pfun,NULL);
		if(!pr)
		{
			printf("call py obj err!\n");
			PyObject* p = PyErr_Occurred();
			if(p)
			{
				PyErr_Print();
			}
		}
		Py_XDECREF(pr);
		// Py_XDECREF(pargs);
		// Py_XDECREF(pfun);

	}
	PyObject* pfun;
};

static PyObject* _engine_start_timer(PyObject* self,PyObject* args)
{
	int t;
	PyObject* pyf;
	int t_id;
	int r = PyArg_ParseTuple(args,"iO",&t,&pyf);
	//add ref pyf??
	if(r && pyf && PyCallable_Check(pyf))
	{
		printf("on _engine start_timer:%d\n",t);
		auto pobj = std::make_shared<PyTimerCb>(pyf);
		u32 timer_id = g_event_loop->start_timer((u32)t,pobj,&PyTimerCb::on_handle);
		return Py_BuildValue("i",timer_id);

		// PyObject* pargs = Py_BuildValue("(i)",t_id);
		// PyObject_CallObject(pyf,pargs);
	}
	else
	{
		printf("parse args error!\n");
		PyErr_SetString(PyExc_RuntimeError,"_engine_start_timer err!!");
		return NULL;
	}
	// Py_RETURN_NONE;
}


static PyMethodDef _engine_methods[] = {
    {"start_timer", _engine_start_timer, METH_VARARGS,"start_timer!!."},
    {"send",        _engine_send,      METH_VARARGS,"do send msg."},
    {NULL,              NULL}           /* sentinel */
};

extern void add_py_sender(PyObject* m);


void init_engine()
{
	PyImport_AddModule("_engine");
	Py_InitModule("_engine",_engine_methods);
	PyObject* pm = PyImport_ImportModule("_engine");
	add_py_sender(pm);
	Py_XDECREF(pm);
}

