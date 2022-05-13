#pragma once
#include "ConnectionSuper.h"

struct ST_SERVER_INIT
{
	WORD wPort;
	std::vector<CConnectionSuper*> connections;
};