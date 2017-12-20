#include "socket.hpp"
#include "handle.hpp"
#include "handles.hpp"
#include "eventloop.hpp"
#include "log.hpp"
#include "connector.hpp"

#include <unistd.h>
#include <map>
#include <vector>
#include <memory>
#include <functional>
#include <string>

int main()
{
	std::string s_ip = "127.0.0.1";
	unsigned short port = 8889;
	log_debug("starting client:%s,%d",s_ip.c_str(),port);
	std::shared_ptr<Client> pclient = std::make_shared<Client>();
	log_debug("start connect....");
	pclient->start_connect(s_ip,port);
	pclient->do_loop();
	log_debug("exiting....");
	return 0;
}



