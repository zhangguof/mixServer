#include "socket.hpp"
#include "handle.hpp"
#include "handles.hpp"
#include "eventloop.hpp"
#include "log.hpp"


#include <unistd.h>
#include <map>
#include <vector>
#include <memory>
#include <functional>
#include <string>

class Connector:public Handle
{
public:
	std::string ip;
	int port;
	

};



