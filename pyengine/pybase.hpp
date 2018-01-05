#ifndef _PY_BASE_H_
#define _PY_BASE_H_
#include "Python.h"
#include "structmember.h"
#include <vector>
//c++ obj
#define INIT_PYCLASS(obj_name) class_<obj_name> obj_name::pyclass = \
							   class_<obj_name>(#obj_name)

#define DEF_PYCLASS(obj_name) static class_<obj_name> pyclass;\
							  static void init_methods();

#define INIT_PYMOD(mod_name) mod_name::get_inst<mod_name>(#mod_name)

#define DEF_PY_METHOD(obj,fname) static PyObject* fname(obj* self,PyObject*args)
#define DEF_PY_METHOD3(obj,fname) static PyObject* fname(obj* self,PyObject* args,PyObject* kwds)

#define PYOBJ_METHOD(obj,fname) PyObject* obj::fname(obj*self,PyObject* args)
#define PYOBJ_METHOD3(obj,fname) PyObject* obj::fname(obj*self,PyObject* args,PyObject*kwds)

#define PYMOD_METHOD(mod,fname) PyObject* mod::fname(PyObject* self,PyObject* args)

#define DEF_PY_GETTER(obj,fname) static PyObject* fname(obj*self,void* closure)
#define DEF_PY_SETTER(obj,fname) static int fname(obj*self PyObject* value, void* closure)
#define PYOBJ_GETTER(obj,fname) PyObject* obj::fname(obj* self,void* closure)
#define PYOBJ_SETTER(obj,fname) int obj::fname(obj*self, PyObject* value, void* closure)

#define INIT_METHOD(name,fun) pyclass.def(name,&fun)

#define INIT_GETSET(name,gfun,sfun) pyclass.def_get_set(name,gfun,sfun)
#define INIT_CALLABLE(fun) pyclass.def_callable(fun)

#define NEW_PYOBJ(obj) class_<obj>::new_obj(NULL,NULL)

int check_py_error();

PyObject* call_py_obj(const char* ,
	const char* ,
	const char* ,...);

template<typename V,typename item_t>
void py_def_push(V& v,const item_t& i)
{
	v.insert(v.end()-1,i);
}

template<typename T>
class class_
{
public:
	typedef PyObject* (*TpyCFun_t)(T*,PyObject*);
	typedef PyObject* (*TpyCFun_t3)(T*,PyObject*,PyObject*);
	typedef PyObject* (*TpyGetter)(T*,void*);
	typedef int (*TpySetter)(T*,PyObject*,void*);
	class_(const char* _name)
	{
		type_name = _name;
		_methods.push_back({0});
		_members.push_back({0});
		_getsetlist.push_back({0});
		py_callable_fun = NULL;
		init_type();
	}
	template<typename fun_t>
	class_<T>& def(const char* fname,fun_t pfun);
	// class_<T>& def(const char* fname,TpyCFun_t pfun);
	class_<T>& def_memb(const char* name,int T::* pmem);
	template<typename getf_t,typename setf_t>
	class_<T>& def_get_set(const char*name,getf_t pget,setf_t pset);
	// class_<T>& def_get_set(const char*name,TpyGetter pget,TpySetter pset);
	class_<T>& def_callable(TpyCFun_t3 pfun);

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
	ternaryfunc get_callable_fun()
	{
		return (ternaryfunc)py_callable_fun;
	}
	std::vector<PyMethodDef> _methods;
	std::vector<PyMemberDef> _members;
	//getter static PyObject *_get(PyObject *self, void *closure)
	//setter static int _set(PyObject *self, PyObject *value, void *closure)
	std::vector<PyGetSetDef> _getsetlist;

	TpyCFun_t3 py_callable_fun;
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
	ptypeobj->tp_call	 = get_callable_fun();
	int n = PyType_Ready(ptypeobj);
	if(n<0)
		ptypeobj = NULL;
}

template<typename T>
template<typename fun_t>
class_<T>& class_<T>::def(const char* fname,fun_t pfun)
{
	assert(_methods.size()>0);
	// auto it = _methods.end();
	// --it;
	py_def_push(_methods,
			PyMethodDef(
			{fname,(PyCFunction)pfun,METH_VARARGS,0}));
	return *this;
}
template<typename T>
template<typename getf_t,typename setf_t>
class_<T>& class_<T>::def_get_set(const char*name,getf_t pget,setf_t pset)
{
	assert(_getsetlist.size()>0);
	py_def_push(_getsetlist,
			PyGetSetDef(
			{(char*)name,(getter)pget,(setter)pset,0}));
	// auto it = _getsetlist.end();
	// --it;
	// *it = {(char*)name,(getter)pget,(setter)pset,0};
	
	// //fix bug!!! _members->_getsetlist
	// //copy is devil!!!
	// _getsetlist.push_back({0,}); 
	return *this;
}

template<typename T>
class_<T>& class_<T>::def_callable(TpyCFun_t3 pfun)
{
	assert(!py_callable_fun);
	py_callable_fun = pfun;
}



template<typename T>
class_<T>& class_<T>::def_memb(
	const char* name,int T::* pmem)
{
	ssize_t off = (ssize_t)&((T*)(0)->*pmem);
	assert(_members.size()>0);
	py_def_push(_members,
		PyMemberDef({(char*)name,T_INT,off,0,0}));
	// auto it = _members.end();
	// --it;
	// *it = {(char*)name,T_INT,off,0,0};
	// _members.push_back({0,});
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
    //No use new,will init some pyobject field.
    //new(self) T();
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
	~pyobj_(){}
	PyObject* get_pyobj()
	{
		return (PyObject*)(this);
	}

	static void init_methods(){};
	static class_<pyobj_> pyclass;

	void* operator new(size_t size,void* p){return p;}
private:
	pyobj_(){}
	// don't use new!
	//just class_<obj>->new_obj();
	//TODO:new placement
	void* operator new(size_t size){return NULL;}
};

class pyTest_:public pyobj_
{
public:
	int n;
	~pyTest_();
	static class_<pyTest_> pyclass;
	static void init_methods();
//methods and setter getter.
	static PyObject* get_n(pyTest_* self,void* closure);
	static int set_n(pyTest_* self,PyObject* val,void* closure);
	static PyObject* test(pyTest_* self,PyObject* args);
};

class pymod
{
// private:
// 	pymod(){}
public:
	template<typename fun_t>
	pymod& def(const char* fname, fun_t pfun);
	virtual void init_methods()=0;
	void init_mod();
	void add_obj(const char* _name,pyobj_* obj);
	PyMethodDef* get_methods()
	{
		return &(*(_methods.begin()));
	}
	PyMemberDef* get_members()
	{
		return &(*(_members.begin()));
	}
	static pymod* p_inst;

	template<typename T>
	static pymod* get_inst(const char* name);

	std::vector<PyMethodDef> _methods;
	std::vector<PyMemberDef> _members;
	const char* name;
	PyObject* mod;
};

template<typename T>
pymod* pymod::get_inst(const char* name)
{
	if(p_inst)
		return p_inst;
	p_inst = new T;
	p_inst->name = name;
	p_inst->_methods.push_back({0,});
	p_inst->_members.push_back({0,});
	p_inst->init_methods();
	p_inst->init_mod();
	return p_inst;
}

template<typename fun_t>
pymod& pymod::def(const char* fname, fun_t pfun)
{
	py_def_push(_methods,
		PyMethodDef(
			{fname,(PyCFunction)pfun,METH_VARARGS,0}));
	return *this;
}

class testmod:public pymod
{

public:
	void init_methods();
	static PyObject* test(PyObject* self,PyObject* args);

};

//_engine mod
class _engine:public pymod
{
public:
	void init_methods();
	DEF_PY_METHOD(PyObject,test);
	DEF_PY_METHOD(PyObject,send);
	DEF_PY_METHOD(PyObject,start_timer);
};




#endif