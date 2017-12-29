#ifndef _UTILS_H_
#define _UTILS_H_
#include <memory>

template<typename T>
class Singleton
{
public:
	typedef std::shared_ptr<T> ptthis_t;
	static ptthis_t get_inst()
	{
		if(p_inst)
		{
			return p_inst;
		}
		else
		{
			p_inst = std::make_shared<T>();
			return p_inst;
		}
	}
	ptthis_t get_this()
	{
		assert(p_inst);
		return p_inst;
	}
private:
	static ptthis_t p_inst;
};

template<typename T>
typename Singleton<T>::ptthis_t Singleton<T>::p_inst = nullptr;

#endif