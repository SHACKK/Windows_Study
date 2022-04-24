#include "pch.h"
#include "CServer.h"

WORD WINAPI ConnectionThread(void* pContext)
{
	SOCKET hConnectionSock = (SOCKET)pContext;

	printf("server connection established.\n");

	{
		int nLength = 0;
		::recv(hConnectionSock, (char*)&nLength, sizeof(nLength), 0); // 처음에 길이 정보를 먼저 받음

		std::string strMsg;
		strMsg.resize(nLength);
		::recv(hConnectionSock, (char*)strMsg.c_str(), nLength, 0); // 실제 문자열을 받음

		printf("server recved msg `%s`.\n", strMsg.c_str());
	}

	Sleep(1000);
	{
		std::string strMsg = "Hello client??";
		int nLength = strMsg.length();
		::send(hConnectionSock, (const char*)&nLength, sizeof(nLength), 0);
		::send(hConnectionSock, strMsg.c_str(), strMsg.length(), 0);
	}

	closesocket(hConnectionSock);
	printf("connection closed\n");
	return 0;
}

DWORD WINAPI ServerThread(void* pContext)
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