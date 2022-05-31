#include "pch.h"
#include "ConnectionSuper.h"

DWORD WINAPI ConnectionThreadCaller(void* pContext)
{
	CConnectionSuper* connection = (CConnectionSuper*)pContext;
	return connection->ConnectionThread();
}

DWORD CConnectionSuper::ConnectionThread()
{
	onConnect();
	onRecv();
	onClose();
	//::closesocket(m_ConnectionSocket);

	return 0;
}

int CConnectionSuper::Establish(SOCKET acceptedSocket, CServer* pServer)
{
	m_ConnectionSocket = acceptedSocket;
	m_pServer = pServer;

	::CreateThread(nullptr, 0, ConnectionThreadCaller, this, 0, nullptr);
	return 0;
}

int CConnectionSuper::Send(LPCBYTE pData, size_t tSize)
{
	int nRet = ::send(m_ConnectionSocket, (const char*)pData, (int)tSize, 0);

	return 0;
}

int CConnectionSuper::Recv(LPBYTE pBuffer, size_t tBufferSize)
{
	int nRet = ::recv(m_ConnectionSocket, (char*)pBuffer, (int)tBufferSize, 0);

	return nRet;
}

int CConnectionSuper::Peek(LPBYTE pBuffer, size_t tBufferSize)
{
	int nRet = ::recv(m_ConnectionSocket, (char*)&pBuffer, (int)tBufferSize, MSG_PEEK);

	return nRet;
}

void CConnectionSuper::SetSocket(SOCKET socket, CServer* server)
{
	m_ConnectionSocket = socket;
	m_pServer = server;
}
