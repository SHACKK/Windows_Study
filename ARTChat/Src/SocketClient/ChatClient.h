#pragma once
#include "Client.h"

class CChatClient : public CClient
{
public:
	std::wstring m_strUserName;
public:
	CChatClient() {};
	~CChatClient()
	{
		Send(CONNECTION_CLOSE_BY_CLIENT);
		this->Close();
	};

	int Send(std::wstring strMessage);
	std::wstring Recv();
	std::vector<std::wstring> RecvChatData();

	void setUserName(std::wstring strName);
	std::wstring getUserName();
};

