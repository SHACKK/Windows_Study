#pragma once
#include "pch.h"
#include "Connection.h"
#include "../SocketClient/_SocketClient.h"
#include "../SocketServer/_SocketServer.h"

DWORD WINAPI AcceptThread(LPVOID pContext)
{
	try
	{
		ST_TEST_PARAM& stTestParam = *(ST_TEST_PARAM*)pContext;
		sockaddr_in service;
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = INADDR_ANY;
		service.sin_port = htons(PORT);
		int nExcep = ::bind(stTestParam.server->GetListenSocket(), (sockaddr*)&service, sizeof(service));
		if (SOCKET_ERROR == nExcep) throw std::exception("Failed to Bind");

		nExcep = listen(stTestParam.server->GetListenSocket(), 200);
		if (SOCKET_ERROR == nExcep) throw std::exception("Failed to Listen");

		sockaddr RemoteInfo;
		int nRemoteInfoSize = (int)sizeof(RemoteInfo);
		SOCKET hConnectionSocket = ::accept(stTestParam.server->GetListenSocket(), &RemoteInfo, &nRemoteInfoSize);

		stTestParam.connection->SetSocket(hConnectionSocket, stTestParam.server);
	}
	catch (std::exception& e)
	{
		printf("%s\n", e.what());
	}

	return 0;
}

DWORD WINAPI ConnectThread(LPVOID pContext)
{
	ST_TEST_PARAM& stTestParam = *(ST_TEST_PARAM*)pContext;
	ST_SERVER_INFO stServerInfo;
	stServerInfo.IP = "127.0.0.1";
	stServerInfo.Port = 56000;
	stTestParam.client->CClient::Connect(stServerInfo);

	return 0;
}

bool MakeConnection(ST_TEST_PARAM stTestParam)
{
	try
	{
		HANDLE hAcceptThread = ::CreateThread(nullptr, 0, AcceptThread, &stTestParam, 0, nullptr);
		HANDLE hConnectThread = ::CreateThread(nullptr, 0, ConnectThread, &stTestParam, 0, nullptr);

		::WaitForSingleObject(hAcceptThread, INFINITE);
		::WaitForSingleObject(hConnectThread, INFINITE);

		return true;
	}
	catch (...)
	{
		printf("[ERROR CODE] %d\n", WSAGetLastError());
		return false;
	}
}