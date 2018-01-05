#include "Python.h"
#include <functional>
#include "handles.hpp"
#include "pybase.hpp"
#include "pyobjs.hpp"


//from pyinit.cpp

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
    INIT_METHOD("write",write);
    INIT_METHOD("flush",flush);
    INIT_METHOD("close",close);
    INIT_CALLABLE(do_call);

}

PYOBJ_GETTER(pySender,get_closed)
{
    if(self->closed)
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
}
pySender::~pySender()
{
    printf("release pySender!!!!\n");
    pstream.reset();//release
}
