#ifndef _PY_BASE_H_
#define _PY_BASE_H_
#include "Python.h"
#include "structmember.h"
#include <vector>
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



template<typename T>
class class_method
{
public:
	class_method();
	class_method<T>& def(const char* fname,PyCFunction pfun);

	class_method<T>& def_memb(const char* name,int T::* pv);
	class_method<T>& def_memb(const char* name,char* T::* pv);
	class_method<T>& _def_memb(const char*name,int type,ssize_t off);

	class_method<T>& def_get_set(const char*name,getter pget,setter pset);

	PyMethodDef* get_methods()
	{
		return &(*(_methods.begin()));
	}
	PyMemberDef* get_members()
	{
		return &(*(_members.begin()));
	}
	PyGetSetDef* get_getsetlist()
	{
		return &(*(_getsetlist.begin()));
	}
	std::vector<PyMethodDef> _methods;
	std::vector<PyMemberDef> _members;
	//getter static PyObject *_get(PyObject *self, void *closure)
	//setter static int _set(PyObject *self, PyObject *value, void *closure)
	std::vector<PyGetSetDef> _getsetlist;
};

class pyobj
{
public:
	typedef class_method<pyobj> method_t; 
	PyObject_HEAD
public:
	void* operator new(size_t size);
	void operator delete(void* p);
	static void dealloc(PyObject* self);
//get set	

//methods

public:
	static const char* name;
	static method_t _methods;
	static void init_methods();
	static pytype* p_type;
};


class pyTest:public pyobj
{
public:
	typedef class_method<pyTest> method_t; 
	int num;
	int num1;
	int num2;
public:
//get set	
	static PyObject* get_num(PyObject* self,void *closure);
	static int set_num(PyObject* self,PyObject* val,void *closure);
//methods
	static PyObject* test(PyObject* self,PyObject *args);
	static PyObject* foo(PyObject* self,PyObject *args);
//from pyobj
	static const char* name;
	void* operator new(size_t size);
	static method_t _methods;
	static void init_methods();
	static pytype* p_type;
};


#endif