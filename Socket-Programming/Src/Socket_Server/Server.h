#pragma once
#include <vector>

class CSocketServer
{
public:
	CSocketServer()
	{
		hListenSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == hListenSocket)
			int nErrorCode = ::WSAGetLastError();


		sockaddr_in service;
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = INADDR_ANY;
		service.sin_port = htons(55000);
		nRet = ::bind(hListenSocket, (sockaddr*)&service, sizeof(service));
		if (SOCKET_ERROR == nRet)
			int nErrorCode = ::WSAGetLastError();
	};

	~CSocketServer() {};

	SOCKET hListenSocket;
	int nRet;
	std::vector<SOCKET> vGeneratedSocket;

	SOCKET WINAPI Listen();
	void Close();
};