#include "Python.h"
#include <functional>
#include "handles.hpp"
#include "pybase.hpp"
#include "pyobjs.hpp"
#include "log.hpp"


//from pyinit.cpp

const char* handle_modname = "httphandle";
void py_new_connect(int conn_id,PyObject* sender)
{
    assert(sender);
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


//new type
//https://docs.python.org/2/extending/newtypes.html

//export interface

pySender* new_pysender(ptstream_t ps)
{
    pySender* p = NEW_PYOBJ(pySender);
    p->init(ps);
    return p;
}

// py sender
// file like obj
//is a PyObject* obj!

INIT_PYCLASS(pySender);

void pySender::init_methods()
{
    // pyclass.def_get_set("closed",&get_closed,0);
    INIT_GETSET("closed",get_closed,0);
    INIT_GETSET("client_addr",get_client_addr,0);
    INIT_GETSET("reading_line",0,set_reading_line);
    INIT_METHOD("write",write);
    INIT_METHOD("flush",flush);
    INIT_METHOD("close",close);
    INIT_CALLABLE(do_call);

}

PYOBJ_GETTER(pySender,get_closed)
{
    if(self->closed || self->pstream->closing || self->pstream->closed)
    {
        Py_RETURN_TRUE;
    }
    else
    {
        Py_RETURN_FALSE;
    }
}

PYOBJ_GETTER(pySender,get_client_addr)
{
    auto psocket = self->pstream->psocket;
    PyObject* addr = Py_BuildValue("(s#i)",
                    psocket->addr.ip.c_str(),
                    psocket->addr.ip.size(),
                    psocket->addr.port
        );
    return addr;
}

PYOBJ_SETTER(pySender,set_reading_line)
{
    if(value == Py_False)
    {
        self->reading_line = false;
    }
    else if(value == Py_True)
    {
        self->reading_line = true;
    }
    else
    {
        PyErr_SetString(PyExc_TypeError,
                        "The last attribute value must be a bool type!");
        log_err("set reading_line err!!!");
        Py_DECREF(value);
        return -1;
    }
    Py_DECREF(value);
    return 0;
}

PYOBJ_METHOD(pySender,close)
{
    self->pstream->close();
    self->closed = true;    
}

PYOBJ_METHOD(pySender,write)
{
    assert(self->pstream); //no init?
    char* s;
    int size;
    PyArg_ParseTuple(args,"s#",&s,&size);
    self->pstream->send(s,size);
    Py_RETURN_NONE;
}
PYOBJ_METHOD(pySender,flush)
{
    //do notthing
    Py_RETURN_NONE;
}

PYOBJ_METHOD3(pySender,do_call)
{
    return self->write(self,args);
}


void pySender::init(const ptstream_t& _ps)
{
    pstream = _ps;
    closed  = false;
    reading_line = true;
}
pySender::~pySender()
{
    printf("release pySender!!!!\n");
    pstream.reset();//release
}
