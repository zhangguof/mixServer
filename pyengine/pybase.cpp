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
	//init methods
	T::init_methods();
	ptypeobj->tp_methods = T::_methods.get_methods();
	ptypeobj->tp_members = T::_methods.get_members();
	ptypeobj->tp_getset  = T::_methods.get_getsetlist();

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

template<typename T>
class_method<T>::class_method()
{
	_methods.push_back({0});
	_members.push_back({0});
	_getsetlist.push_back({0});
}

template<typename T>
class_method<T>& class_method<T>::def(const char* fname,PyCFunction pfun)
{
	auto it = _methods.end();
	--it;
	*it = {fname,pfun,METH_VARARGS,0};
	_methods.push_back({0,});
	return *this;
}

template<typename T>
class_method<T>& class_method<T>::def_memb(
	const char* name,int T::* pmem)
{
	ssize_t off = (ssize_t)&((T*)(0)->*pmem);
	return _def_memb(name,
		T_INT,off);
}

template<typename T>
class_method<T>& class_method<T>::def_memb(
	const char* name,char* T::* pmem)
{
	ssize_t off = (ssize_t)&((T*)(0)->*pmem);
	return _def_memb(name,
		T_STRING,off);
}

template<typename T>
class_method<T>& class_method<T>::_def_memb(
	const char*name,int type,ssize_t off)
{
	assert(_members.size()>0);
	auto it = _members.end();
	--it;
	*it = {(char*)name,type,off,0,0};
	_members.push_back({0,});
	return *this;
}

template<typename T>
class_method<T>& class_method<T>::def_get_set(const char*name,getter pget,setter pset)
{
	assert(_getsetlist.size()>0);
	auto it = _getsetlist.end();
	--it;
	*it = {(char*)name,pget,pset,0};
	_members.push_back({0,});
	return *this;
}

//==============================================
//pyobj
const char* pyobj::name = "pyobj";
pytype* pyobj::p_type = nullptr;

class_method<pyobj> pyobj::_methods = class_method<pyobj>();
void pyobj::init_methods()
{
	
}

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


// pyobj::pyobj(){}

// pyobj::~pyobj(){}

void pyobj::dealloc(PyObject* self)
{
	Py_TYPE(self)->tp_free(self);
}

//===========================================
//pyTest
//static member.
const char* pyTest::name = "pytest";
pytype* pyTest::p_type = nullptr;
class_method<pyTest> pyTest::_methods = class_method<pyTest>();

//do overwrite
void* pyTest::operator new(size_t size)
{
	if(!p_type)
		p_type = init_type<pyTest>();
	assert(p_type);
	assert(size == p_type->type.tp_basicsize);
	return p_type->_new_obj(NULL,NULL);
}

void pyTest::init_methods()
{
	_methods.def("test",&test)
			.def("foo",&foo);
	_methods.def_memb("num1",&pyTest::num1)
			.def_memb("num2",&pyTest::num2);
	_methods.def_get_set("num",&get_num,&set_num);
}

PyObject* pyTest::test(PyObject* self,PyObject *args)
{
	int a;
	char* s;
	PyArg_ParseTuple(args,"is",&a,&s);
	printf("test:%s\n",s);
	return PyInt_FromLong(42+a);
}

PyObject* pyTest::foo(PyObject* self,PyObject *args)
{
	return PyInt_FromLong(43);
}


PyObject* pyTest::get_num(PyObject* self,void *closure)
{
	auto pobj = (pyTest*) self;
	PyObject* re = PyInt_FromLong(pobj->num);
	if(re)
		Py_INCREF(re);
	return re;
}

int pyTest::set_num(PyObject* self,PyObject* val,void *closure)
{
	((pyTest*)self)->num = PyInt_AS_LONG(val);
	return 0;

}






