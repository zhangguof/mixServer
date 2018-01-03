#ifndef _PY_BASE_H_
#define _PY_BASE_H_
#include "Python.h"
#include "structmember.h"
#include <vector>
//c++ obj
#define INIT_PYCLASS(obj_name) class_<obj_name> obj_name::pyclass = class_<obj_name>(#obj_name)

template<typename T>
class class_
{
public:
	typedef PyObject* (*TpyCFun_t)(T*,PyObject*);
	typedef PyObject* (*TpyGetter)(T*,void*);
	typedef int (*TpySetter)(T*,PyObject*,void*);
	class_(const char* _name)
	{
		type_name = _name;
		_methods.push_back({0});
		_members.push_back({0});
		_getsetlist.push_back({0});
		init_type();
	}
	template<typename fun_t>
	class_<T>& def(const char* fname,fun_t pfun);
	// class_<T>& def(const char* fname,TpyCFun_t pfun);
	class_<T>& def_memb(const char* name,int T::* pmem);
	template<typename getf_t,typename setf_t>
	class_<T>& def_get_set(const char*name,getf_t pget,setf_t pset);
	// class_<T>& def_get_set(const char*name,TpyGetter pget,TpySetter pset);


	static void dealloc(PyObject* self);
	void init_type();

	T *_new_obj(PyObject *args, PyObject *kwds);
	static T* new_obj(PyObject* args, PyObject* kwds);
	// class_<T>& def(const char* fname,PyCFunction pfun);

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
	const char* type_name;
	PyTypeObject* ptypeobj;
};


template<typename T>
void class_<T>::dealloc(PyObject* self)
{

	((T*)self)->~T();
	Py_TYPE(self)->tp_free(self);
}

template<typename T>
void class_<T>::init_type()
{
	assert(!ptypeobj);
	ptypeobj = new PyTypeObject;
	memset(ptypeobj,0,sizeof(PyTypeObject));
	
	ptypeobj->ob_refcnt = 0;
	ptypeobj->ob_type = NULL;
	ptypeobj->tp_name = type_name;
	ptypeobj->tp_basicsize = sizeof(T);
	ptypeobj->tp_dealloc = dealloc;
	ptypeobj->tp_flags = Py_TPFLAGS_DEFAULT;
	T::init_methods();
	ptypeobj->tp_methods = get_methods();
	ptypeobj->tp_members = get_members();
	ptypeobj->tp_getset  = get_getsetlist();
	int n = PyType_Ready(ptypeobj);
	if(n<0)
		ptypeobj = NULL;
}

template<typename T>
template<typename fun_t>
class_<T>& class_<T>::def(const char* fname,fun_t pfun)
{
	auto it = _methods.end();
	--it;
	*it = {fname,(PyCFunction)pfun,METH_VARARGS,0};
	_methods.push_back({0,});
	return *this;
}
template<typename T>
template<typename getf_t,typename setf_t>
class_<T>& class_<T>::def_get_set(const char*name,getf_t pget,setf_t pset)
{
	assert(_getsetlist.size()>0);
	auto it = _getsetlist.end();
	--it;
	*it = {(char*)name,(getter)pget,(setter)pset,0};
	
	//fix bug!!! _members->_getsetlist
	//copy is devil!!!
	_getsetlist.push_back({0,}); 
	return *this;
}

template<typename T>
class_<T>& class_<T>::def_memb(
	const char* name,int T::* pmem)
{
	ssize_t off = (ssize_t)&((T*)(0)->*pmem);
	assert(_members.size()>0);
	auto it = _members.end();
	--it;
	*it = {(char*)name,T_INT,off,0,0};
	_members.push_back({0,});
	return *this;
}

template<typename T>
T* class_<T>::_new_obj(PyObject *args, PyObject *kwds)
{
	T *self;
	// if(!ptypeobj)
	// 	init_type();
	PyTypeObject* ptype = ptypeobj;
	assert(ptype);
	assert(ptype->tp_alloc);
    self = (T *)ptype->tp_alloc(ptype, 0);
    new(self) T();
    return self;
}

template<typename T>
T* class_<T>::new_obj(PyObject* args, PyObject* kwds)
{
	return T::pyclass._new_obj(args,kwds);
}

class pyobj_
{
public:
	PyObject_HEAD
	// typedef class_<pyobj_>::obj_ obj_t;
	pyobj_();
	~pyobj_()
	{
		//relase by python.
		// Py_XDECREF(_obj);
		printf("free pyobj_.....\n");
	}
	int num;
	
	static PyObject* get_num(pyobj_* self,void *closure);
	static int set_num(pyobj_* self,PyObject* val,void *closure);
	// static get_num(PyObject* self,void *closure)
	static void init_methods();
	static PyObject* test(pyobj_* self, PyObject* args);
	static class_<pyobj_> pyclass;
	void* operator new(size_t size,void* p){return p;}
private:
	// don't use new!
	//just class_<obj>->new_obj();
	//TODO:new placement
	void* operator new(size_t size){}
	// PyObject* _obj;
};

class pyTest_:public pyobj_
{
public:
	int n;
	pyTest_();
	~pyTest_();
	static class_<pyTest_> pyclass;
	static void init_methods();
	static PyObject* get_n(pyTest_* self,void* closure);
	static int set_n(pyTest_* self,PyObject* val,void* closure);
};


#endif