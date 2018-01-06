#ifndef _LOG_H_
#define _LOG_H_
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#define NONE                 "\e[0m"
#define RED                  "\e[0;31m"
#define GREEN                "\e[0;32m"


#if defined(DEBUG)
#define log_debug(msg,args...) Log::debug(__FILE__,__LINE__,msg,##args)
#else
#define log_debug(msg,args...) ;
#endif
// #define log_debug(fmt,args...) printf(fmt,##args)
#define log_err(msg,args...) Log::err(__FILE__,__LINE__,msg,##args)

class Log
{
public:
	static const int buf_size = 1024;
	static void format_log(char*buf,const char* tag,
		const char* filename,int fileline,const char* msg);

	static void debug(const char* filename,int fileline,
		const char* msg,...);

	static void err(const char* filename,int fileline,
		const char* msg,...);
};

#endif