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
		connection.Peek((LPBYTE)&header, sizeof(header));
		if (header.MagicOK() && header.tSize == 0) // GetType()을 써서 종료신호를 보내는 패킷인지를 확인해야함
			break;

		connection.onRecv();
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

int CConnectionSuper::Peek(LPBYTE pBuffer, size_t BufferSize)
{
	try
	{
		::recv(m_ConnectionSocket, (char*)&pBuffer, (int)BufferSize, MSG_PEEK);
	}
	catch (std::exception& e)
	{
		printf("[Receve Error] : %s", e.what());
	}
	return 0;
}
