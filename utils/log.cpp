#include "log.hpp"

void Log::format_log(char*buf,const char* tag,
	const char* filename,int fileline,const char* msg)
{
	time_t now;
	struct tm* tm_now;
	time(&now);
	tm_now = localtime(&now);
	char datetime[100];
	sprintf(datetime,"%d-%d-%d %d:%d:%d",
		tm_now->tm_year+1900,tm_now->tm_mon+1,
		tm_now->tm_mday,tm_now->tm_hour,tm_now->tm_min,
		tm_now->tm_sec);
	sprintf(buf,"[%s][%s][FILE:%s:%d:]%s\n",tag,datetime,filename,fileline,msg);
}

void Log::debug(const char* filename,int fileline,
	const char* msg,...)
{
	char buf[buf_size];
	char msgbuf[buf_size];
	va_list args;
	va_start(args,msg);
	vsprintf(msgbuf,msg,args);
	va_end(args);
	// printf("[%s][FILE:%s:%d:]%s\n",datetime,filename,fileline,msgbuf);
	format_log(buf,"debug",filename,fileline,msgbuf);
	printf("%s%s%s",GREEN,buf,NONE);
}
void Log::err(const char* filename,int fileline,
	const char* msg,...)
{
	char buf[buf_size];
	char msgbuf[buf_size];
	va_list args;
	va_start(args,msg);
	vsprintf(msgbuf,msg,args);
	va_end(args);
	format_log(buf,"error",filename,fileline,msgbuf);
	printf("%s%s%s",RED,buf,NONE);
}