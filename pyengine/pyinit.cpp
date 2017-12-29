#include "Python.h"

#include "stdio.h"
const char* py_str = "import sys\n"\
					 "sys.path.insert(0,'./script')\n"\
					 "import init\n"\
					 "init.init()\n";

void inittest();

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

	Py_Initialize();
	PySys_SetArgvEx(argc, argv, 0);
	// inittest();

}
void run_py()
{
	/* Execute some Python statements (in module __main__) */
    PyRun_SimpleString(py_str);
}

int init_py(int argc,char** argv)
{
	init_python(argc,argv);
	// printf("Hello !!!\n");
	run_py();
	init_pb_mod();
	printf("init pb mod success!\n");
	PyRun_SimpleString("import main\nmain.main()\n");
	// Py_Exit(0);
	return 0;
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

//static module
void inittest()
{
	PyImport_AddModule("test");
	Py_InitModule("test",test_methods);
	// PyImport_AddModule("_api_implementation");
	// Py_InitModule("_api_implementation",)
}

