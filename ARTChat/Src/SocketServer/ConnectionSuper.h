#pragma once

class CServer;

class CConnectionSuper
{
private:
	SOCKET m_ConnectionSocket;
	CServer* m_pServer;
	void ConnectionThread();
	sockaddr RemoteInfo;

protected:
	virtual void onConnect() = 0;
	virtual void onRecv() = 0;
	virtual void onClose() = 0;

public:
	int Establish(SOCKET acceptedSocket, CServer* pServer);
	int Send(LPCBYTE pBuffer, size_t BufferSize);
	int Recv(LPBYTE pBuffer, size_t BufferSize);
};

