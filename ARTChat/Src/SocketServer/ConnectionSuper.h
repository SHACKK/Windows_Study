#pragma once
#include "Server.h"

class CConnectionSuper
{
private:
	CServer* m_pServer;
	SOCKET m_Socket;
	void ConnectionThread();

protected:
	virtual void onConnect() = 0;
	virtual void onRecv() = 0;
	virtual void onClose() = 0;

public:
	int Establish(SOCKET acceptedSocket, CServer* pServer);
	int Send(const LPBYTE pBuffer, size_t BufferSize);
	int Recv(LPBYTE pBuffer, size_t BufferSize);
};

