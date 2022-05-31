#pragma once
#include "pch.h"
#include "Connection.h"
#include "../SocketClient/_SocketClient.h"
#include "../SocketServer/_SocketServer.h"

DWORD WINAPI AcceptThread(LPVOID pContext)
{
	ST_ACCEPTTHREAD_PARAM& stParam = *(ST_ACCEPTTHREAD_PARAM*)pContext;
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;
	service.sin_port = htons(PORT);
	int nExcep = ::bind(stParam.server->GetListenSocket(), (sockaddr*)&service, sizeof(service));
	if (SOCKET_ERROR == nExcep) throw std::exception("Failed to Bind");

	nExcep = listen(stParam.server->GetListenSocket(), 200);
	if (SOCKET_ERROR == nExcep) throw std::exception("Failed to Listen");

	sockaddr RemoteInfo;
	int nRemoteInfoSize = (int)sizeof(RemoteInfo);
	SOCKET hConnectionSocket = ::accept(stParam.server->GetListenSocket(), &RemoteInfo, &nRemoteInfoSize);

	stParam.connection->SetSocket(hConnectionSocket, stParam.server);

	return 0;
}

DWORD WINAPI ConnectThread(LPVOID pContext)
{
	CChatClient& client = *(CChatClient*)pContext;
	ST_SERVER_INFO stServerInfo;
	stServerInfo.IP = "127.0.0.1";
	stServerInfo.Port = 56000;
	client.Connect(stServerInfo);

	return 0;
}

void MakeConnection(CServer* server, CChatConnection* connection, CChatClient* client)
{
	try
	{
		ST_ACCEPTTHREAD_PARAM stArg;
		stArg.server = server;
		stArg.connection = connection;
		HANDLE hAcceptThread = ::CreateThread(nullptr, 0, AcceptThread, &stArg, 0, nullptr);
		HANDLE hConnectThread = ::CreateThread(nullptr, 0, ConnectThread, client, 0, nullptr);

		::WaitForSingleObject(hAcceptThread, INFINITE);
		::WaitForSingleObject(hConnectThread, INFINITE);
	}
	catch (...)
	{
		printf("[ERROR] Connection Failed [ERROR-CODE] %d", WSAGetLastError());
	}
}