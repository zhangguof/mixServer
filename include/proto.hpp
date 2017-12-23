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
	Msg();
	Msg(int _len);
	Msg(std::string s);
	Msg(const char*p, int size);
	void init_str()
	{
		praw_data = std::make_shared<std::string>();
	}
	void clear();
	void try_write(int _len);
	void read(const char*p, int size);
	void write(const char*p, int size);
	void write(const std::string& s);
	
	std::string get_raw()
	{
		char buf[sizeof(int)];
		pack_int(buf,len);
		return std::string(buf,sizeof(int)) + *praw_data;
	}
	ptsting_t get_data()
	{
		return praw_data;
	}
	int get_remain_len()
	{
		return len - has_write_size;
	}
	int len;
	bool msg_reading;
private:
	ptsting_t praw_data;
	int has_write_size;


};
#endif