#pragma once
#include "../SocketServer/_SocketServer.h"
#include "../SocketClient/_SocketClient.h"

#define PORT 56000
#define MAX_CONNECTION 1

struct ST_ACCEPTTHREAD_PARAM
{
	CServer* server;
	CChatConnection* connection;
};

DWORD WINAPI AcceptThread(LPVOID pContext);
DWORD WINAPI ConnectThread(LPVOID pContext);
bool MakeConnection(CServer* server, CChatConnection* connection, CChatClient* client);