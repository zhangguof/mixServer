#ifndef _ERRORS_H
#define _ERRORS_H
#include <errno.h>
#include <cstring>


const char* get_error_msg(int eno);

#endif