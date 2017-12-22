#ifndef _PROTO_H
#define _PROTO_H
#include <string>
#include <memory>

template<typename T>
void pack_int(char*p,T a)
{
	int size = sizeof(T);
	// T* pa = &a;
	int aa = a;
	char* src = reinterpret_cast<char*>(&aa);
	std::copy(src,src+size,p);
}

class Msg
{
public:
	typedef std::shared_ptr<std::string> ptsting_t;
	typedef std::shared_ptr<Msg> ptmsg_t;
	Msg(int _len);
	Msg(std::string s);
	Msg(const char*p, int size);
	void read(const char*p, int size);
	void write(const char*p, int size);
	void write(std::string s);

	int len;
	std::string get_raw()
	{
		char buf[sizeof(int)];
		// printf("===test:%d:%d\n",len,*((int*)buf));
		pack_int(buf,len);
		// printf("===test222:%d:%d\n",len,*((int*)buf));
		return std::string(buf,sizeof(int)) + *praw_data;
	}
	ptsting_t get_data()
	{
		return praw_data;
	}

private:
	
	ptsting_t praw_data;

};
#endif