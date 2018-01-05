#ifndef _MEM_H_
#define _MEM_H_
#include <memory>
#include <vector>
#include<execinfo.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <cassert>
#include <iostream>
#include <map>
// #include <cxxabi.h>

#if defined(DEBUG_MEM)
#define STD std_
#else
#define STD std

#endif

namespace std_{

// void print_stack()
// {
// 	int j, nptrs;
// 	const int SIZE = 100;
//  	void *buffer[SIZE];
//  	char **strings;
//  	nptrs = backtrace(buffer, SIZE);
//  	printf("backtrace() returned %d addresses\n", nptrs);

//  	strings = backtrace_symbols(buffer, nptrs);
//  	std::string strs[nptrs];
//  	std::string line = "";
// 	for (j = 0; j < nptrs; j++)
// 	{
// 		 strs[j] = strings[j];
// 	}
//    for(auto s:strs)
//    {
//    	auto pos = s.find(" _");
//    	if(pos!=std::string::npos)
//    	{
//    		s.insert(pos+1,"_");
//    	}
//    	line.append(s+"\n");
//    }
//    	printf("%s\n",line.c_str());
//    free(strings);
// }


class Refinfo
{
public:
	Refinfo(void* pobj){
		inc_refs.clear();
		dec_refs.clear();
		ref_cnt = 0;
		
		assert(ref_infos.find(pobj)==ref_infos.end());
		ref_infos[pobj] = this;
		// printf("add map:%0x,%0x\n",pobj,this);
	}
	std::vector<std::string> inc_refs;
	std::vector<std::string> dec_refs;
	int ref_cnt;
public:
	void add_call_info(std::vector<std::string>& v);

	void add_inc()
	{
		// printf("=====add_inc!!!!!\n");
		++ref_cnt;
		add_call_info(inc_refs);
	}
	void add_dec()
	{
		--ref_cnt;
		add_call_info(dec_refs);
	}

	void print_ref_trace(std::vector<std::string>& v)
	{
		// printf("do pritnf ref trace!\n");
		for(auto it:inc_refs)
		{
			printf("%s\n",it.c_str());
			printf("=======================\n");
		}
	}
	void print_inc_refs()
	{
		print_ref_trace(inc_refs);
	}
	void print_dec_refs()
	{
		print_ref_trace(dec_refs);
	}
	static Refinfo* get_refinfo(void* p)
	{
		auto it = ref_infos.find(p);
		assert(it!=ref_infos.end());
		// printf("get_refinfo:ref_info_addr:%0x,pobj:%0x\n",it->second,p);
		return it->second;

	}
	static std::map<void*,Refinfo*> ref_infos;
};



template<typename T>
class shared_ptr:public std::shared_ptr<T>
{
public:
	//defult
	shared_ptr():std::shared_ptr<T>(){
		ref_info = NULL;
	}
	//normal
	template<class _Yp> shared_ptr(_Yp*);
	//assin construct
    shared_ptr<T>& operator=(const shared_ptr<T>& r) ;
    //copy construct
	shared_ptr<T>(const shared_ptr<T>& r):std::shared_ptr<T>(r){
		printf("in construct1!!!\n");
		this->ref_info = r.ref_info;
		assert(ref_info);
		ref_info->add_inc();
	}
	// shared_ptr<T>(const std::shared_ptr<T>& r):std::shared_ptr<T>(r)
	// {
	// 	printf("in construct2,from std:pt!!!\n");
	// 	this->ref_info = r.ref_info;
	// 	ref_info->add_inc();
	// }
	shared_ptr<T>(const std::shared_ptr<T> r):std::shared_ptr<T>(r)
	{
		printf("in construct3,from std:pt!!!\n");
		this->ref_info =  Refinfo::get_refinfo((void*)r.get());
		ref_info->add_inc();
		// printf("ref_info:%0x!!!ref_cnt:%d,ref_cnt_addr:%0x\n",
		// ref_info,ref_info->ref_cnt,&(ref_info->ref_cnt)
		// );

	}
	~shared_ptr(){

		assert(ref_info);
		ref_info->add_dec();
	}

 //    template<class Y> shared_ptr<T>(const shared_ptr<Y>& r):std::shared_ptr<T>(r){
	// printf("in construct2!!!\n");
	// 	assert(ref_info);
	// 	ref_info->add_inc();
	// }

 //    template<class Y> shared_ptr<T>(shared_ptr<Y>&& r):std::shared_ptr<T>(r){
 //    printf("in construct3!!!\n");
 //    print_stack();
 //    	if(!ref_info)
 //    		ref_info = new Refinfo;
 //    	assert(ref_info);
 //    	printf("add_inc!!!\n");
 //   	 	ref_info->add_inc();
	// }

	// long use_count() const // never throws
	// {

	// 	return std::shared_ptr<T>::use_count();
	// }
	//rewrite
    // shared_ptr<T>& operator=(const shared_ptr<T>& r) ;
    // template<class Y> shared_ptr<T>& operator=(const shared_ptr<Y>& r) ;
    // shared_ptr<T>& operator=(shared_ptr<T>&& r) ;
    // template<class Y> shared_ptr<T>& operator=(shared_ptr<Y>&& r);


	template<typename A1>
	static shared_ptr<T> make_shared(A1& a1)
	{
		return shared_ptr<T>(new T(a1));
	}
	template<typename A1,typename A2>
	static shared_ptr<T> make_shared(A1& a1,A2& a2)
	{
		return shared_ptr<T>(new T(a1,a2));
	}
	template<typename A1,typename A2,typename A3>
	static shared_ptr<T> make_shared(A1& a1,A2& a2,A3& a3)
	{
		return shared_ptr<T>(new T(a1,a2,a3));
	}

	static shared_ptr<T> make_shared()
	{
		return shared_ptr<T>(new T);
	}
public:
	Refinfo* ref_info;
	void print_inc_refs()
	{
		assert(ref_info);
		ref_info->print_inc_refs();
	}
	void print_dec_refs()
	{
		assert(ref_info);
		ref_info->print_dec_refs();
	}
	int get_refcnt() const{
		assert(ref_info);
		return ref_info->ref_cnt;
	}
	std::shared_ptr<T>& get_std_pt()
	{
		return static_cast<std::shared_ptr<T> >(&this);
	}
};



template<class T>
template<class _Yp>
shared_ptr<T>::shared_ptr(_Yp* __p):std::shared_ptr<T>(__p)
{
	ref_info = new Refinfo(__p);
	printf("new ref_info!!!ref_cnt:%d,ref_cnt_addr:%0x\n",
		ref_info->ref_cnt,&(ref_info->ref_cnt)
		);
	ref_info->add_inc();
}


template<typename T>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr<T>& r)
{
	printf("1test in operator=!!\n");
	std::shared_ptr<T>::operator=(r);
	this->ref_info = r.ref_info;
	ref_info->add_inc();
	return *this;
}

// template<typename T,typename t>
// shared_ptr<T>& static_pointer_cast(std::shared_ptr<t>& ele)
// {
// 	//base type=t
// 	std::shared_ptr<T> pele = std::static_pointer_cast<T>(ele);
// 	shared_ptr<T> p_new = shared_ptr<T>(pele);
// 	return p_new;
// }

template<typename T,typename t>
shared_ptr<T>& static_pointer_cast(std::shared_ptr<t> ele)
{
	//base type=t
	std::shared_ptr<T> pele = std::static_pointer_cast<T>(ele);
	shared_ptr<T> *p_new = new shared_ptr<T>(pele);
	return *p_new;
}

// template<typename T>
// template<class Y> shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr<Y>& r)
// {
// 	printf("2test in operator=!!\n");
// 	std::shared_ptr<T>::operator=(r);
// 	return *this;

// }
// template<typename T>
// shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr<T>&& r){
// 	printf("3test in operator=!!\n");
// 	std::shared_ptr<T>::operator=(r);
// 	return *this;
// }

// template<typename T>
// template<class Y> shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr<Y>&& r){
// 	printf("4test in operator=!!\n");
// 	std::shared_ptr<T>::operator=(r);
// 	return *this;
// }


template<typename T,typename A1,typename A2,typename A3>
shared_ptr<T> make_shared(A1& a1,A2& a2,A3& a3)
{
    return shared_ptr<T>::make_shared(a1,a2,a3);
}

template<typename T,typename A1,typename A2>
shared_ptr<T> make_shared(A1& a1,A2& a2)
{
    return shared_ptr<T>::make_shared(a1,a2);
}

template<typename T,typename A1>
shared_ptr<T> make_shared(A1& a1)
{
    return shared_ptr<T>::make_shared(a1);
}

template<typename T>
shared_ptr<T> make_shared()
{
    return shared_ptr<T>::make_shared();
}

};

#endif