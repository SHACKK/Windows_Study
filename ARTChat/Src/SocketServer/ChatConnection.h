#pragma once
#include "ConnectionSuper.h"

class CChatConnection : protected CConnectionSuper
{
public:
	void onConnect();
	void onRecv();
	void onClose();
};

