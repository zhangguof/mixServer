#include "Python.h"
#include <memory>
#include "proto.hpp"
#include "services.hpp"

#include "stdio.h"
const char* py_str = "import sys\n"\
					 "sys.path.insert(0,'./script')\n"\
					 "import init\n"\
					 "init.init()\n";
char* pyhome = "./script";

void inittest();
void init_sender();
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

void init_python(int argc,char** argv)
{
	Py_NoSiteFlag = 1;
	Py_SetProgramName(argv[0]);
	Py_SetPythonHome(pyhome);

	Py_Initialize();
	PySys_SetArgvEx(argc, argv, 0);
	// inittest();
	init_sender();

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
	run_py();
	init_pb_mod();

	//main.main()
	PyRun_SimpleString("import main\nmain.main()\n");
	// Py_Exit(0);
	g_py_init = 1;
	return 0;
}

void handle_pb_msg(int s_id,int c_id,int uid,const char* s)
{
	if(!g_py_init)
	{
		printf("py not init!!!!");
		return;
	}
	//imoprt proto;
	//proto.HandleMsg(sid,cid,pbstr)
	PyObject* pmod = PyImport_ImportModule("proto");
	PyObject* pfun = PyObject_GetAttrString(pmod,"HandleMsg");
	PyObject* args = Py_BuildValue("(iiis)",s_id,c_id,uid,s);
	if(args && pfun && PyCallable_Check(pfun))
	{
		PyObject* pr =  PyObject_CallObject(pfun,args);
		if(!pr)
		{
			printf("call HandleMsg err!\n");
			PyObject* p = PyErr_Occurred();
			if(p)
				PyErr_Print();
		}
	}
	else
	{
		printf("handle_msg error!:%d,%d\n",s_id,c_id);
	}
	Py_XDECREF(args);
	Py_XDECREF(pfun);
	Py_XDECREF(pmod);
	// PyObject* args = PyTuple_New(3);
	// PyObject* pv = PyInt_FromLong(s_id);
	// PyTuple_SetItem(args,0,pv); //steal the ref.
	
	// pv = PyInt_FromLong(c_id);
	// PyTuple_SetItem(args,1,pv);
	

}

static PyObject* _sender_send(PyObject* self,PyObject* args)
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

static PyObject* test_foo(PyObject* self,PyObject* args)
{
	return PyInt_FromLong(42);
}

static PyMethodDef test_methods[] = {
    {"foo",             test_foo,      METH_NOARGS,
     "Return the meaning of everything."},
    {NULL,              NULL}           /* sentinel */
};

static PyMethodDef _sender_methods[] = {
    {"send",             _sender_send,      METH_VARARGS,
     "do send!!."},
    {NULL,              NULL}           /* sentinel */
};

//static module
void inittest()
{
	PyImport_AddModule("test");
	Py_InitModule("test",test_methods);
}

void init_sender()
{
	PyImport_AddModule("_sender");
	Py_InitModule("_sender",_sender_methods);
}

