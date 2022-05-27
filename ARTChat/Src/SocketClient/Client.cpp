#include "pch.h"
#include "Client.h"

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
		return 0;
	}
	return 1;
}

int CClient::Close()
{
	//::closesocket(m_hClientSocket);

	return 0;
}

int CClient::Send(std::wstring strMessage)
{
	size_t nLength = strMessage.length() * sizeof(wchar_t);
	::send(m_hClientSocket, (const char*)&nLength, sizeof(nLength), 0);
	::send(m_hClientSocket, (const char*)strMessage.c_str(), nLength, 0);
	return 0;
}

int CClient::Recv(LPBYTE pBuffer)
{
	int nRet = 0;
	try
	{
		int nSize = 0;
		nRet = ::recv(m_hClientSocket, (char*)&nSize, (int)sizeof(nSize), 0);
		nRet = ::recv(m_hClientSocket, (char*)&pBuffer, nSize, 0);
	}
	catch (...)
	{
		printf("[Receve Error] : %d", WSAGetLastError());
	}
	return nRet;
}

std::vector<std::wstring> CClient::RecvChatData()
{
	std::vector<std::wstring> vecChatData;
	size_t nVecSize;
	::recv(m_hClientSocket, (char*)&nVecSize, sizeof(size_t), 0);
	vecChatData.resize(nVecSize);

	for (size_t i = 0; i < nVecSize; i++)
	{
		int nMsgLength;
		::recv(m_hClientSocket, (char*)&nMsgLength, sizeof(nMsgLength), 0);
		vecChatData[i].resize(nMsgLength / sizeof(wchar_t));
		::recv(m_hClientSocket, (char*)vecChatData[i].c_str(), nMsgLength, 0);
	}

	return vecChatData;
}

std::wstring CClient::Recv()
{
	size_t nLength = 0;
	::recv(m_hClientSocket, (char*)&nLength, sizeof(nLength), 0);

	std::wstring strRet;
	strRet.resize(nLength / sizeof(wchar_t));
	::recv(m_hClientSocket, (char*)strRet.c_str(), nLength, 0);

	return strRet;
}

int CClient::Peek(LPBYTE pBuffer, size_t tBufferSize)
{
	int nRet = 0;
	try
	{
		nRet = ::recv(m_hClientSocket, (char*)&pBuffer, (int)tBufferSize, MSG_PEEK);
	}
	catch (...)
	{
		printf("[Receve Error] : %d", WSAGetLastError());
	}
	return nRet;
}