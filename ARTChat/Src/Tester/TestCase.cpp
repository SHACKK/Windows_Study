#pragma once
#include "pch.h"
#include "TestCase.h"
#include "Connection.h"
#include "../SocketClient/_SocketClient.h"
#include "../SocketServer/_SocketServer.h"

bool SendTest()
{
	try
	{
		CServer server;
		CChatConnection connection;
		CChatClient client;
		MakeConnection(&server, &connection, &client);

		std::wstring strSendMessage = L"TEST";
		client.Send(strSendMessage);
		std::wstring strRecvMessage = connection.Recv();
		return true;
	}
	catch (...)
	{
		printf("[ERROR] %d", WSAGetLastError());
		return false;
	}
}
