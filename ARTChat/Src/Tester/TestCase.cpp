#pragma once
#include "pch.h"
#include "TestCase.h"
#include "Connection.h"
#include "../SocketClient/_SocketClient.h"
#include "../SocketServer/_SocketServer.h"

bool SendRecvTest()
{
	try
	{
		CServer server;
		CChatConnection connection;
		CChatClient client;
		MakeConnection(&server, &connection, &client);

		std::wstring strSendMessage = L"TEST";
		int nRet = client.Send(strSendMessage);
		if (0 < nRet)	return false;

		std::wstring strRecvMessage = connection.Recv();
		if (strRecvMessage.empty() || wcscmp(strRecvMessage.c_str(), strSendMessage.c_str()))	return false;

		nRet = connection.Send(strSendMessage);
		if (0 < nRet)	return false;

		strRecvMessage = client.Recv();
		if (strRecvMessage.empty() || wcscmp(strRecvMessage.c_str(), strSendMessage.c_str()))	return false;

		return true;
	}
	catch (...)
	{
		printf("[ERROR] %d", WSAGetLastError());
		return false;
	}
}
