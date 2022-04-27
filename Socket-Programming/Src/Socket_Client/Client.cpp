#include "pch.h"
#include "Client.h"

bool CSocketClient::Connect()
{
	hClientSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == hClientSocket)
	{
		return false;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(55000);
	
	int nRet = ::connect(hClientSocket, (sockaddr*)&service, sizeof(service));
	if (SOCKET_ERROR == nRet)
	{
		return false;
	}

	return true;
}

void CSocketClient::Send(std::wstring strMsg)
{
	int nLength = strMsg.length();
	::send(hClientSocket, (const char*)&nLength, sizeof(nLength), 0);
	::send(hClientSocket, (const char*)strMsg.c_str(), nLength, 0);
}

std::wstring CSocketClient::Recv()
{
	int nLength = 0;
	::recv(hClientSocket, (char*)&nLength, sizeof(nLength), 0);

	std::wstring strRet;
	strRet.resize(nLength);
	::recv(hClientSocket, (char*)strRet.c_str(), nLength, 0);

	return strRet;
}

void CSocketClient::Close()
{
	if(INVALID_SOCKET != hClientSocket)
		::closesocket(hClientSocket);
}
