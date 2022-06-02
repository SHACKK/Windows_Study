#include "pch.h"
#include "Client.h"

int CClient::Connect(ST_SERVER_INFO stServerInfo)
{
	try
	{
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
		return 0;
	}
	return 1;
}

int CClient::Close()
{
	
	::closesocket(m_hClientSocket);
	return 0;
}

int CClient::Send(LPBYTE pData, size_t tSize)
{
	int nRet = ::send(m_hClientSocket, (const char*)&pData, (int)tSize, 0);
	return nRet;
}

int CClient::Recv(LPBYTE pBuffer, size_t tSize)
{
	int nRet = ::recv(m_hClientSocket, (char*)&pBuffer, (int)tSize, 0);
	return nRet;
}

int CClient::Peek(LPBYTE pBuffer, size_t tBufferSize)
{
	int	nRet = ::recv(m_hClientSocket, (char*)&pBuffer, (int)tBufferSize, MSG_PEEK);
	return nRet;
}