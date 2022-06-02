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
	bool Connect(ST_SERVER_INFO stServerInfo, std::wstring strUserName);
	int Send(std::wstring strMessage);
	std::wstring Recv();
	std::vector<std::wstring> RecvChatData();

	void setUserName(std::wstring strName);
	std::wstring getUserName();

	//Test¿ë
	std::vector<std::wstring> m_vecChatData;
};

