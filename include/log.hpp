#ifndef _LOG_H_
#define _LOG_H_
#include <stdio.h>
#include <time.h>
#include <stdarg.h>


#define log_debug(msg,args...) Log::debug(__FILE__,__LINE__,msg,##args)
// #define log_debug(fmt,args...) printf(fmt,##args)

class Log
{
public:

	static void debug(const char* filename,int fileline,
		const char* msg,...)
	{
		time_t now;
		struct tm* tm_now;
		time(&now);
		tm_now = localtime(&now);
		char datetime[100];
		char msgbuf[1024];
		sprintf(datetime,"%d-%d-%d %d:%d:%d",
			tm_now->tm_year+1900,tm_now->tm_mon+1,
			tm_now->tm_mday,tm_now->tm_hour,tm_now->tm_min,
			tm_now->tm_sec);

		va_list args;
		va_start(args,msg);
		vsprintf(msgbuf,msg,args);
		va_end(args);


		printf("[%s][FILE:%s:%d:]%s\n",datetime,filename,fileline,msgbuf);
	}
};

#endif