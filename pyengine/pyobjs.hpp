#ifndef _PYOBJ_H_
#define _PYOBJ_H_
#include "pybase.hpp"
#include "handles.hpp"

class pySender:public pyobj_
{
public:
    ptstream_t pstream;
    bool closed;
    bool reading_line;
    ~pySender();
    void init(const ptstream_t& _ps);

    // static class_<pyTest_> pyclass;
    // static void init_methods();
    DEF_PYCLASS(pySender);

    //getter setter
    DEF_PY_GETTER(pySender,get_closed);
    DEF_PY_GETTER(pySender,get_client_addr);
    DEF_PY_SETTER(pySender,set_reading_line);

    DEF_PY_METHOD(pySender,flush);
    //DEF_PY_METHOD(getvalue);
    // DEF_PY_METHOD(pySender,isatty);
    //DEF_PY_METHOD(read)  ;
    //DEF_PY_METHOD(readline);
    //DEF_PY_METHOD(readlines);
    //DEF_PY_METHOD(reset);
    // DEF_PY_METHOD(seek) ;
    // DEF_PY_METHOD(tell);
    // DEF_PY_METHOD(truncate);
    DEF_PY_METHOD(pySender,close);
    DEF_PY_METHOD(pySender,write);
    // DEF_PY_METHOD(writelines);
    DEF_PY_METHOD3(pySender,do_call);

};


#endif