#include "pch.h"
#include "Client.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 55000

int CClient::Connect(ST_SERVER_INFO stServerInfo)
{
	try
	{
		m_hClientSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == m_hClientSocket) throw std::exception("Failed to Create SOCKET");

		sockaddr_in service;
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = inet_addr(stServerInfo.IP);
		service.sin_port = htons(stServerInfo.Port);

		int nConnectionResult = ::connect(m_hClientSocket, (sockaddr*)&service, sizeof(service));
		if (SOCKET_ERROR == nConnectionResult) throw std::exception("Failed to Connect");
	}
	catch (std::exception& e)
	{
		printf("[ERROR] : %s\n", e.what());
		return 1;
	}
	return 0;
}

int CClient::Close()
{
	return 0;
}

int CClient::Send(LPCBYTE pData, size_t Size)
{
	return 0;
}

int CClient::Recv(LPBYTE pBuffer, size_t BufferSize)
{
	return 0;
}
