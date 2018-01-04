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

int g_py_init = 0;

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

void init_mods()
{
	// _engine::get_inst<_engine>("_engine");
	INIT_PYMOD(_engine);
}

void init_python(int argc,char** argv)
{
	Py_NoSiteFlag = 1;
	Py_SetProgramName(argv[0]);
	Py_SetPythonHome(pyhome);

	Py_Initialize();
	PySys_SetArgvEx(argc, argv, 0);
	
	// init_engine();
	init_mods();

}

void add_script_path()
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
	add_script_path(); //for search script!
	init_pb_mod();
	g_py_init = 1;

	//main.main()
	PyRun_SimpleString("import main\nmain.main()\n");

	return 0;
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

