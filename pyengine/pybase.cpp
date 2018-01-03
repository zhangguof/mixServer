#include "Python.h"
#include "pybase.hpp"
#include "structmember.h"


template<typename T>
static pytype* init_type()
{
	assert(!T::p_type);
	printf("init type:%s\n",T::name);

	pytype* p_type = new pytype;
	memset(p_type,0,sizeof(pytype));
	PyTypeObject* ptypeobj = p_type->get_type();

	ptypeobj->ob_refcnt = 0;
	ptypeobj->ob_type = NULL;
	ptypeobj->tp_name = T::name;
	ptypeobj->tp_basicsize = sizeof(T);
	ptypeobj->tp_dealloc = T::dealloc;
	// ptypeobj->tp_new = T::_new;
	ptypeobj->tp_flags = Py_TPFLAGS_DEFAULT;
	ptypeobj->tp_methods = T::_methods;
	ptypeobj->tp_members = T::_members;
	ptypeobj->tp_getset   = T::_getsetlist;

	int n = PyType_Ready(ptypeobj);
	if(n<0)
		return NULL;
	return p_type;
}
PyTypeObject* pytype::get_type()
{
	return &type;
}

PyObject * pytype::_new_obj(PyObject *args, PyObject *kwds)
{
	PyObject *self;
	PyTypeObject* ptype = &type;
	assert(ptype);
	assert(ptype->tp_alloc);
    self = ptype->tp_alloc(ptype, 0);
    return self;
}
//pyobj
const char* pyobj::name = "pyobj";

PyMethodDef pyobj::_methods[] = {
	// PY_VAR_METHOD_DEF(test,"pyobj test!"),
	{NULL}
};

PyMemberDef pyobj::_members[] = {
	//{"num",T_INT,offsetof(pyobj,num),0,"test num!"},
	// PY_MEM_DEF(pyobj,num,T_INT,"pyobj int member!"),
    {NULL}  /* Sentinel */
};

PyGetSetDef pyobj::_getsetlist[] = {
	PY_GETTER(num,pyobj::get_num,"get num!!"),
    // {"num", (getter), NULL, "True if the file is closed"},
    {0},
};

PyObject* pyobj::get_num(PyObject* self,void *closure)
{
	PyObject* re = PyInt_FromLong(42);
	if(re)
		Py_INCREF(re);
	return re;
}


pytype* pyobj::p_type = nullptr;

void* pyobj::operator new(size_t size)
{
	if(!p_type)
		p_type = init_type<pyobj>();
	assert(p_type);
	assert(size == p_type->type.tp_basicsize);

	return p_type->_new_obj(NULL,NULL);
}

void pyobj::operator delete(void* p)
{
	Py_XDECREF(p);
}

// PyObject* pyobj::test(PyObject* self,PyObject *args,PyObject *kwds)
// {
// 	int a;
// 	char* s;
// 	PyArg_ParseTuple(args,"is",&a,&s);
// 	return PyInt_FromLong(42+a);
// }

// pyobj::pyobj(){}

// pyobj::~pyobj(){}

void pyobj::dealloc(PyObject* self)
{
	Py_TYPE(self)->tp_free(self);
}


const char* pyTest::name = "pytest";
PyMethodDef pyTest::_methods[] = {
	PY_VAR_METHOD_DEF(test2,"pytest test222!"),
	{NULL}
};

PYOBJ_FUN(pyTest,test2)
{
	int a;
	int b;
	PyArg_ParseTuple(args,"ii",&a,&b);
	return PyInt_FromLong(a+b);
}

PyMemberDef pyTest::_members[] = {
	PY_MEM_DEF(pyTest,xxx,T_INT,"pytest int member!"),
    {NULL}  /* Sentinel */
};

pytype* pyTest::p_type = nullptr;


void* pyTest::operator new(size_t size)
{
	if(!p_type)
		p_type = init_type<pyTest>();
	assert(p_type);
	assert(size == p_type->type.tp_basicsize);

	return p_type->_new_obj(NULL,NULL);
}





