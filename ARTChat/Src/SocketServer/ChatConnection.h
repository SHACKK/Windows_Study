#pragma once
#include "ConnectionSuper.h"

class CChatConnection : public CConnectionSuper
{
public:
	void onConnect();
	void onRecv();
	void onClose();
};

