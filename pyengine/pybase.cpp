#include "Python.h"
#include "pybase.hpp"
#include "structmember.h"

//========


// class_<pyobj_> pyobj_::pyclass = class_<pyobj_>("pyobj_");
INIT_PYCLASS(pyobj_);
pyobj_::pyobj_()
{
	num = 100;
	printf("pyobj_ init.........\n");
	// _obj = pyclass._new_obj(this,NULL,NULL);
}

void pyobj_::init_methods()
{
	pyclass.def("test",&test);
	pyclass.def_get_set("num",&get_num,&set_num);

}


PyObject* pyobj_::get_num(pyobj_* self,void *closure)
{
	PyObject* re = PyInt_FromLong(self->num);
	if(re)
		Py_INCREF(re);
	return re;
}

int pyobj_::set_num(pyobj_* self,PyObject* val,void *closure)
{
	self->num = PyInt_AS_LONG(val);
	return 0;

}

PyObject* pyobj_::test(pyobj_* self, PyObject* args)
{
	int a;
	char *s;
	int size;
	PyArg_ParseTuple(args,"is#",&a,&s,&size);
	printf("test!!!!:%d:%s\n",a,s);
	Py_RETURN_NONE;
}


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

int pyTest_::set_n(pyTest_* self,PyObject* val,void* closure)
{
	self->n = PyInt_AS_LONG(val);
	return 0;
}
