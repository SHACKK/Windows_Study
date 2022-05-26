#pragma once
#include "Server.h"

class CConnectionSuper
{
private:
	SOCKET m_ConnectionSocket;

protected:
	CServer* m_pServer;
	virtual void onConnect() = 0;
	virtual void onRecv() = 0;
	virtual void onClose() = 0;

public:
	DWORD ConnectionThread();
	int Establish(SOCKET acceptedSocket, CServer* pServer);
	int SendChatData(std::vector<std::wstring> vecChatData);
	int Send(std::wstring strMessage);
	std::wstring Peek();
	std::wstring Recv();
};
