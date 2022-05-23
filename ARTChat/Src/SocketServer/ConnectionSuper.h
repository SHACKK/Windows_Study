#pragma once
#include "Server.h"

class CConnectionSuper
{
private:
	SOCKET m_ConnectionSocket;
	sockaddr m_RemoteInfo;

protected:
	CServer* m_pServer;
	virtual void onConnect() = 0;
	virtual void onRecv() = 0;
	virtual void onClose() = 0;

public:
	DWORD ConnectionThread();
	int Establish(SOCKET acceptedSocket, CServer* pServer);
	int Send(LPCBYTE pBuffer, size_t BufferSize);
	int Recv(LPBYTE pBuffer, size_t BufferSize);
	int Peek(LPBYTE pBuffer, size_t BufferSize);
};
