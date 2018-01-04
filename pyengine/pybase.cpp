#include "Python.h"
#include "pybase.hpp"
#include "structmember.h"
//utils fun
int check_py_error()
{
	PyObject* p = PyErr_Occurred();
	if(p)
	{
		PyErr_Print();
		return 1;
	}
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
	if(args_fmt)
	{
		va_list vargs;
		va_start( vargs, args_fmt );
		args = Py_VaBuildValue( args_fmt, vargs );
		va_end(vargs);
	}
	else
	{
		args = NULL;
	}

	if(( (args_fmt && args)||(!args_fmt && !args)) 
		&& pfun && PyCallable_Check(pfun))
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

//=======================
//pyobj
// class_<pyobj_> pyobj_::pyclass = class_<pyobj_>("pyobj_");
INIT_PYCLASS(pyobj_);


//==================================
//pyTest

// class_<pyTest_> pyTest_::pyclass = class_<pyTest_>("pytest_");
INIT_PYCLASS(pyTest_);
pyTest_::pyTest_()
{
	n = 111;
	printf("init....\n");
}
pyTest_::~pyTest_()
{
	printf("free pytest_\n");
}

void pyTest_::init_methods()
{
	pyclass.def_get_set("n",&get_n,&set_n);
	pyclass.def("test",test);
}

PyObject* pyTest_::get_n(pyTest_* self,void* closure)
{
	PyObject* re = PyInt_FromLong(self->n);
	if(re)
		Py_INCREF(re);
	return re;
}

PyObject* pyTest_::test(pyTest_* self, PyObject* args)
{
	int a;
	char *s;
	int size;
	PyArg_ParseTuple(args,"is#",&a,&s,&size);
	printf("test!!!!:%d:%s\n",a,s);
	Py_RETURN_NONE;
}

int pyTest_::set_n(pyTest_* self,PyObject* val,void* closure)
{
	self->n = PyInt_AS_LONG(val);
	return 0;
}

//========================================
//pymod
pymod* pymod::p_inst = nullptr;

void pymod::init_mod()
{
	mod = Py_InitModule(name, get_methods());
}

void pymod::add_obj(const char* _name,pyobj_* obj)
{
	//steals a reference to value
	//int PyModule_AddObject(PyObject *module, const char *name, PyObject *value
	assert(mod);
	int n = PyModule_AddObject(mod,_name,obj->get_pyobj());
	assert(n==0);
}

//test mod
void testmod::init_methods()
{
	def("test",&test);
}

PyObject* testmod::test(PyObject* self,PyObject* args)
{
	int a;
	char *s;
	int size;
	PyArg_ParseTuple(args,"is#",&a,&s,&size);
	printf("testmod do test!!!!:%d:%s\n",a,s);
	Py_RETURN_NONE;
}





