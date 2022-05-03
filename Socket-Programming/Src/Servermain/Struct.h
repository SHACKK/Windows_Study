#pragma once
#include <Windows.h>
#include "../MessageQueue/Message.h"
#include "../Socket_Server/Socket_Server.h"

struct ST_WSA_INITIALIZER
{
	WSAData wsa;
	ST_WSA_INITIALIZER(void)
	{
		WSAStartup(MAKEWORD(2, 2), &wsa);
	}
	~ST_WSA_INITIALIZER(void)
	{
		WSACleanup();
	}
};

struct ST_CONNECTION_PARAM
{
	CSocketServer* server;
	CSocketConnection connect;
	CMessage* msg;
};