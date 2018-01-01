#include "Python.h"
#include <functional>

//from pyinit.cpp
PyObject* call_py_obj(const char* modname,
	const char* fun_name,
	const char* args_fmt,...);

const char* handle_modname = "httphandle";
void py_new_connect(int conn_id,PyObject* sender)
{
	call_py_obj(handle_modname,"new_connect","(iO)",conn_id,sender);
}

void py_handle_line(int conn_id,const char* s,int size)
{
	call_py_obj(handle_modname,"handle_line","(is#)",
		conn_id,s,size);
}
void py_handle_message(int conn_id, const char* s,int size)
{
	call_py_obj(handle_modname,"handle_message","(is#)",
		conn_id,s,size);
}

void py_handle_close(int conn_id)
{
	call_py_obj(handle_modname,"handle_close","(i)",conn_id);
}

typedef std::function<void(const char*,int)> send_raw_t;


//new type
//https://docs.python.org/2/extending/newtypes.html

typedef struct {
    PyObject_HEAD
    /* Type-specific fields go here. */
   send_raw_t* p_sender;
} pySender;

static void
pySender_dealloc(pySender* self)
{
    delete self->p_sender;
    Py_TYPE(self)->tp_free((PyObject*)self);
}


static PyObject * pySender_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    pySender *self;

    self = (pySender *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->p_sender = nullptr;
    }

    return (PyObject *)self;
}

static int
pySender_init(pySender *self, send_raw_t* p)
{
	self->p_sender = p;
	return 0;
}
//PyObject_Call(PyObject *callable_object, PyObject *args, PyObject *kw)
static PyObject *
pySender_call(pySender* self,PyObject *args, PyObject *kw)
{
	if(!self->p_sender)
		return NULL;
    char* s;
    int size;
    int r = PyArg_ParseTuple(args,"s#",
        &s,&size); 
    if(!r)
    {
        printf("pySender_call err!\n");
        PyErr_SetString(PyExc_RuntimeError,"pySender_call err!!");
        return NULL;
    }
	(*(self->p_sender))(s,size);
    Py_RETURN_NONE;
}

static PyMethodDef pySender_methods[] = {
    {"name", (PyCFunction)pySender_call, METH_NOARGS,
     "do sender raw"
    },
    {NULL}  /* Sentinel */
};



static PyTypeObject pySenderType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pySender",             		/* tp_name */
    sizeof(pySender), 				/* tp_basicsize */
    0,                         		/* tp_itemsize */
    (destructor)pySender_dealloc,	/* tp_dealloc */
    0,                         		/* tp_print */
    0,                         		/* tp_getattr */
    0,                         		/* tp_setattr */
    0,                         		/* tp_compare */
    0,                         		/* tp_repr */
    0,                         		/* tp_as_number */
    0,                         		/* tp_as_sequence */
    0,                         		/* tp_as_mapping */
    0,                         		/* tp_hash */
    (ternaryfunc)pySender_call,    	/* tp_call */
    0,                         		/* tp_str */
    0,                         		/* tp_getattro */
    0,                         		/* tp_setattro */
    0,                         		/* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE |
        Py_TPFLAGS_HAVE_GC,    		/* tp_flags */
    "pySender objects",           	/* tp_doc */
    0,//(traverseproc)Noddy_traverse,   /* tp_traverse */
    0,//(inquiry)Noddy_clear,           /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    pySender_methods,             /* tp_methods */
    0,//Noddy_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,//(initproc)Noddy_init,      /* tp_init */
    0,                         /* tp_alloc */
    pySender_new,                 /* tp_new */
};

// static PyMethodDef pySender_methods[] = {
//     {NULL}  /* Sentinel */
// };

PyObject* new_pysender(send_raw_t* p)
{
    PyObject* self =  pySender_new(&pySenderType,NULL,NULL);
    pySender_init((pySender*)self,p);
    return self;
}

void add_py_sender(PyObject* m)
{
    //pySenderType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&pySenderType) < 0)
        return;
    Py_INCREF(&pySenderType);
    PyModule_AddObject(m, "pySender", (PyObject *)&pySenderType);
}
