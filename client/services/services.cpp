#include "services.hpp"

void Services::init(Sender s)
{
	sender = s;
	Test::get_inst()->init();
}

