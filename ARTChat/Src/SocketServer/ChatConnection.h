#pragma once
#include "ConnectionSuper.h"

class CChatConnection : public CConnectionSuper
{
private:
	std::wstring strUserName;
public:
	void onConnect();
	void onRecv();
	void onClose();
};