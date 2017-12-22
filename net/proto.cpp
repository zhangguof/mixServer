#include "log.hpp"
#include "buffer.hpp"
#include "proto.hpp"

Msg::Msg(int _len):len(_len){}
Msg::Msg(std::string s)
{
	write(s);
}
Msg::Msg(const char*p ,int size)
{
	write(p,size);
};

void Msg::read(const char*p,int size)
{
	// praw_data = std::make_shared<std::string>(p,size);
}

//init msg!!
void Msg::write(std::string s)
{
	len = s.size();
	praw_data = std::make_shared<std::string>(s);
}

void Msg::write(const char* p,int size)
{
	len = size;
	praw_data = std::make_shared<std::string>(p,size);
}