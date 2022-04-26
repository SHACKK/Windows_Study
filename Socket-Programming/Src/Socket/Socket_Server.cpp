#include "pch.h"
#include "Socket_Server.h"

SOCKET WINAPI CSocketServer::Listen()
{
	SOCKET hListenSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == hListenSocket)
	{
		int nErrorCode = ::WSAGetLastError();
		return -1;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;
	service.sin_port = htons(55000);
	int nRet = ::bind(hListenSocket, (sockaddr*)&service, sizeof(service));
	if (SOCKET_ERROR == nRet)
	{
		return -1;
	}

	nRet = ::listen(hListenSocket, 200);
	if (SOCKET_ERROR == nRet)
	{
		return -1;
	}

	while (true)
	{
		sockaddr RemoteInfo;
		int nRemoteInfoSize = (int)sizeof(RemoteInfo);
		SOCKET hConnectionSock = ::accept(hListenSocket, &RemoteInfo, &nRemoteInfoSize);
		if (INVALID_SOCKET == hConnectionSock)
		{
			return -1;
		}

		vGeneratedSocket.push_back(hConnectionSock);
		return hConnectionSock;
	}
}

void CSocketServer::Close()
{
}
