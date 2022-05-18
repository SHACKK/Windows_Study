#include "pch.h"
#include "ConnectionSuper.h"

void CConnectionSuper::ConnectionThread()
{
	onConnect();
	while (true)
	{
		onRecv();
	}
	onClose();
}

int CConnectionSuper::Establish(SOCKET acceptedSocket, CServer* pServer)
{
	m_ConnectionSocket = acceptedSocket;
	m_pServer = pServer;

	ConnectionThread();
	//::CreateThread(nullptr, 0, ConnectionThread, nullptr, 0, nullptr);
	return 0;
}

int CConnectionSuper::Send(LPCBYTE pBuffer, size_t BufferSize)
{
	try
	{
		::send(m_ConnectionSocket, (const char*)&pBuffer, (int)BufferSize, 0);
	}
	catch (std::exception& e)
	{
		printf("[Send Error] : %s", e.what());
		return 1;
	}
	return 0;
}

int CConnectionSuper::Recv(LPBYTE pBuffer, size_t BufferSize)
{
	try
	{
		::recv(m_ConnectionSocket, (char*)&pBuffer, (int)BufferSize, 0);
	}
	catch (std::exception& e)
	{
		printf("[Receve Error] : %s", e.what());
	}
	return 0;
}
