#ifndef _PY_BASE_H_
#define _PY_BASE_H_
#include "Python.h"
//c++ obj
//replace PyObject* by pyobj* ?
#define PY_VAR_METHOD_DEF(name,doc) {#name,(PyCFunction)name,METH_VARARGS,doc}
#define PY_MEM_DEF(objname,name,type,doc) {#name,type,offsetof(objname,name),0,doc}
#define PY_GETTER(name,func,doc) {#name,(getter)func,NULL,doc}

// #define PY_VAL_METHOD(obj,name) PyObject* obj::namePyObject* self,PyObject *args,PyObject *kwds
#define PYOBJ_FUN_DEF(name) static PyObject* name(PyObject* self,PyObject* args);
#define PYOBJ_FUN(obj,name) PyObject* obj::name(PyObject* self,PyObject* args)

class pytype
{
public:
	PyTypeObject* get_type();
	PyTypeObject type;
	PyObject * _new_obj(PyObject *args, PyObject *kwds);
};

class pyobj
{
public:
	PyObject_HEAD
public:
	void* operator new(size_t size);
	void operator delete(void* p);
	static void dealloc(PyObject* self);
	//methods
	static PyObject* get_num(PyObject* self,void *closure);
	// static PyObject* test(PyObject* self,PyObject *args,PyObject *kwds);
public:
	static const char* name;
	static PyMethodDef _methods[];
	static PyMemberDef _members[];
	static PyGetSetDef _getsetlist[];
	static pytype* p_type;

};

class pyTest:public pyobj
{
public:
	void* operator new(size_t size);
public:
	int xxx;
	PYOBJ_FUN_DEF(test2)
	static const char* name;
	static PyMethodDef _methods[];
	static PyMemberDef _members[];
	
	static pytype* p_type;
};

// class pySender:public pyobj
// {
// public:
// 	void* operator new(size_t size);
// public:
// 	int xxx;
// 	PYOBJ_FUN_DEF(test2)
// 	static const char* name;
// 	static PyMethodDef _methods[];
// 	static PyMemberDef _members[];
// 	static pytype* p_type;
// };


#endif