#include "pch.h"
#include "ConnectionSuper.h"
#include "ChatConnection.h"
#include "../MessagePacket/_Packet.h"

DWORD WINAPI CConnectionSuper::ConnectionThread(LPVOID pContext)
{
	CConnectionSuper& connection = *(CConnectionSuper*)pContext;
	CPacketHeader header;

	connection.onConnect();
	while (true)
	{
		connection.Recv((LPBYTE)&header, sizeof(header), MSG_PEEK);
		if (header.MagicOK() && header.tSize == 0)
			break;

		connection.onConnect();
	}
	connection.onClose();
	return 0;
}

int CConnectionSuper::Establish(SOCKET acceptedSocket, CServer* pServer)
{
	m_ConnectionSocket = acceptedSocket;
	m_pServer = pServer;

	::CreateThread(nullptr, 0, this->ConnectionThread, this, 0, nullptr);
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

int CConnectionSuper::Recv(LPBYTE pBuffer, size_t BufferSize, int flags = 0)
{
	try
	{
		::recv(m_ConnectionSocket, (char*)&pBuffer, (int)BufferSize, flags);
	}
	catch (std::exception& e)
	{
		printf("[Receve Error] : %s", e.what());
	}
	return 0;
}
