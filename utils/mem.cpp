#include "mem.hpp"
#if defined(DEBUG_MEM)


namespace std_{

auto Refinfo::ref_infos = std::map<void*,Refinfo*>();

//char * __cxxabiv1::__cxa_demangle (const char *__mangled_name, 
//		char *__output_buffer, size_t *__length, int *__status)
void Refinfo::add_call_info(std::vector<std::string>& v)
{
	int j, nptrs;
	const int SIZE = 100;
 	void *buffer[SIZE];
 	char **strings;
 	nptrs = backtrace(buffer, SIZE);
 	// printf("backtrace() returned %d addresses\n", nptrs);

 	strings = backtrace_symbols(buffer, nptrs);
 	std::string strs[nptrs];
 	std::string line = "";
	for (j = 0; j < nptrs; j++)
	{
		 strs[j] = strings[j];
	}
   for(auto s:strs)
   {
   	auto pos = s.find(" _");
   	if(pos!=std::string::npos)
   	{
   		s.insert(pos+1,"_");
   	}
   	line.append(s+"\n");

   }
   v.push_back(std::string(line));
   free(strings);
}

}
#endif
