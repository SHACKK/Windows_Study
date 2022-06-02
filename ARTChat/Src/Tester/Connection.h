#pragma once
#include "../SocketServer/_SocketServer.h"
#include "../SocketClient/_SocketClient.h"

#define PORT 56000

struct ST_TEST_PARAM
{
	CServer* server;
	CChatConnection* connection;
	CChatClient* client;
};

DWORD WINAPI AcceptThread(LPVOID pContext);
DWORD WINAPI ConnectThread(LPVOID pContext);
bool MakeConnection(ST_TEST_PARAM stTestParam);