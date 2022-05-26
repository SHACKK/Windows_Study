#pragma once
#include "ConnectionSuper.h"

#define CONNECTION_CLOSE_BY_CLIENT L"/CloseByClient"
#define CONNECTION_CLOSE_BY_SERVER L"/CloseByServer"

class CChatConnection : public CConnectionSuper
{
private:
	std::wstring strUserName;
public:
	void onConnect();
	void onRecv();
	void onClose();
};