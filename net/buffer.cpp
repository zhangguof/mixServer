#include "buffer.hpp"


void Buffer::make_write_space(int len)
{
	if(writeable_size()>=len)
		return;
	if(pre_size() + writeable_size() >= len)
	{
		std::copy(date.begin()+start_idx,date.begin()+end_idx,date.begin());
		end_idx -= start_idx;
		start_idx = 0;
		return;
	}
	date.resize(size_all() + len);
}
void Buffer::write(const char* buf,int len)
{
	make_write_space(len);
	std::copy(buf,buf+len,end());
	end_idx+=len;
}

void Buffer::write(std::string s)
{
	write(s.c_str(),s.size());
}
//read all
char* Buffer::read()
{
	if(readable_size()<=0)
		return NULL;
	char* r = get_buf();
	clear();
	return r;
}
char* Buffer::read(int n)
{
	if(readable_size()<=0)
		return NULL;
	char* r = get_buf();
	start_idx += n;
	return r;
}
int Buffer::read_int()
{
	assert(readable_size()>=4);
	int *r = reinterpret_cast<int*>(get_buf());
	start_idx += sizeof(int);
	return *r;
}
std::string Buffer::read_str(int _size)
{
	assert(readable_size()>=_size);
	auto r = std::string(get_buf(),_size);
	start_idx += _size;
	return r;
}

int Buffer::find_CRLF()
{
	int size = readable_size();
	if(size<2)
	{
		return -1;
	}
	auto pbuf = get_buf();
	for(int i=0;i<size-1;++i)
	{
		if(pbuf[i]=='\r' && pbuf[i+1]=='\n')
			return i;
	}
	return -1;
}