#include "log.hpp"
#include "buffer.hpp"
#include "proto.hpp"

Msg::Msg():len(0),msg_reading(false),has_write_size(0)
{
	init_str();
}
Msg::Msg(int _len):len(_len),msg_reading(false){
	init_str();
}
Msg::Msg(std::string s)
{
	init_str();
	len = s.size();
	write(s);
}
Msg::Msg(const char*p ,int size)
{
	init_str();
	len = size;
	write(p,size);
};
void Msg::clear()
{
	len = 0;
	has_write_size = 0;
	msg_reading = false;
	praw_data->clear();

}
void Msg::try_write(int _len)
{
	assert(_len>0);
	len = _len;
	has_write_size = 0;
	msg_reading = true;
	log_debug("set praw_data....:%d,%d",_len,praw_data?1:0);
	assert(praw_data);
	praw_data->reserve(_len);
	praw_data->clear();
}

void Msg::read(const char*p,int size)
{
	// praw_data = std::make_shared<std::string>(p,size);
}

//init msg!!
void Msg::write(const std::string& s)
{
	// len = s.size();
	// praw_data = std::make_shared<std::string>(s);
	praw_data->append(s);
	has_write_size += s.size();
}

void Msg::write(const char* p,int size)
{
	// len = size;
	// praw_data = std::make_shared<std::string>(p,size);
	praw_data->append(p,size);
	has_write_size += size;
}