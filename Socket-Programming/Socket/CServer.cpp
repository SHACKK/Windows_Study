#include "pch.h"
#include "CServer.h"

DWORD WINAPI CServer::ServerThread(void* pContext)
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
		sockaddr remoteInfo;
		int nRemoteInfoSize = (int)sizeof(remoteInfo);
		SOCKET hConnectionSock = ::accept(hListenSocket, &remoteInfo, &nRemoteInfoSize);
		if (INVALID_SOCKET == hConnectionSock)
		{
			return -1;
		}

		HANDLE hThread = CreateThread(nullptr, 0, ConnectionThread, (LPVOID)hConnectionSock, 0, nullptr);
	}

	return 0;
}
