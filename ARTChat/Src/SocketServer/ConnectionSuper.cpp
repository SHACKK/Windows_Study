#include "pch.h"
#include "ConnectionSuper.h"
#include "ChatConnection.h"

DWORD WINAPI ConnectionThreadCaller(void* pContext)
{
	CConnectionSuper* connection = (CConnectionSuper*)pContext;
	return connection->ConnectionThread();
}

DWORD CConnectionSuper::ConnectionThread()
{
	std::string strContext;
	strContext.resize(10);

	onConnect();
	while (true)
	{
		int nRecvSize = Peek((LPBYTE)&strContext, sizeof(strContext));
		if (nRecvSize == 0)	break;

		onRecv();
	}

	m_pServer->DisConnect(this);
	onClose();
	return 0;
}

int CConnectionSuper::Establish(SOCKET acceptedSocket, CServer* pServer)
{
	m_ConnectionSocket = acceptedSocket;
	m_pServer = pServer;

	::CreateThread(nullptr, 0, ConnectionThreadCaller, this, 0, nullptr);
	return 0;
}

int CConnectionSuper::Send(LPCBYTE pBuffer, size_t BufferSize)
{
	int nRet= 0;
	try
	{
		nRet = ::send(m_ConnectionSocket, (const char*)&pBuffer, (int)BufferSize, 0);
	}
	catch (std::exception& e)
	{
		printf("[Send Error] : %s", e.what());
		return 1;
	}
	return nRet;
}

int CConnectionSuper::Recv(LPBYTE pBuffer, size_t BufferSize)
{
	int nRet= 0;
	try
	{
		nRet = ::recv(m_ConnectionSocket, (char*)&pBuffer, (int)BufferSize, 0);
	}
	catch (std::exception& e)
	{
		printf("[Receve Error] : %s", e.what());
	}
	return nRet;
}

int CConnectionSuper::Peek(LPBYTE pBuffer, size_t BufferSize)
{
	int nRet= 0;
	try
	{
		nRet = ::recv(m_ConnectionSocket, (char*)&pBuffer, (int)BufferSize, MSG_PEEK);
	}
	catch (std::exception& e)
	{
		printf("[Receve Error] : %s", e.what());
	}
	return nRet;
}
