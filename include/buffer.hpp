#ifndef _BUFFER_H_
#define _BUFFER_H_
#include <vector>
#include <string>
#include <cassert>
#include <memory>
typedef unsigned int u32;

class Buffer
{
public:
	static const int BUFFER_SIZE = 4096;
	typedef std::shared_ptr<Buffer> ptbuffer_t;
	Buffer()
	{
		start_idx = 0;
		end_idx  = 0;
		date.resize(BUFFER_SIZE);

	}
	int size_all()
	{
		return end_idx;
	}
	void clear()
	{
		start_idx = 0;
		end_idx = 0;
	}
	int pre_size()
	{
		return start_idx;
	}
	int readable_size()
	{
		return end_idx - start_idx;
	}
	void has_read(int n)
	{
		assert(start_idx+n<=end_idx);
		start_idx += n;
	}
	int writeable_size()
	{
		return date.capacity() - end_idx;
	}
	void has_write(int n)
	{
		assert(end_idx + n <= date.capacity());
		end_idx += n;
	}
	char* end()
	{
		return begin() + end_idx;
	}
	char *begin()
	{
		return &(*date.begin());
	}
	char* get_buf()
	{
		return &(*(date.begin()+start_idx));
	}
	void make_write_space(int len);
	void write(const char* buf,int len);
	void write(std::string s);
	char* read();
	char* read(int n);
	int read_int();
	std::string read_str(int _size);
	~Buffer(){}
private:
	std::vector<char> date;
	int start_idx;
	int end_idx;	
};




#endif