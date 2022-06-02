#pragma once
#include "pch.h"
#include "TestCase.h"
#include "Connection.h"
#include "../SocketClient/_SocketClient.h"
#include "../SocketServer/_SocketServer.h"

bool Client_SendTest(ST_TEST_PARAM stTestParam)
{
	try
	{
		int nRet = stTestParam.client->Send(L"TEST");
		std::wstring strTmp = stTestParam.connection->Recv();
		if (nRet <= 0)	return false;

		return true;
	}
	catch (...)
	{
		printf("[ERROR CODE] %d", WSAGetLastError());
		return false;
	}
}

bool Connection_SendTest(ST_TEST_PARAM stTestParam)
{
	try
	{
		int nRet = stTestParam.connection->Send(L"TEST");
		std::wstring strTmp = stTestParam.client->Recv();
		if (nRet <= 0)	throw;

		return true;
	}
	catch (...)
	{
		printf("[ERROR CODE] %d", WSAGetLastError());
		return false;
	}
}

bool Client_RecvTest(ST_TEST_PARAM stTestParam)
{
	try
	{
		stTestParam.connection->Send(L"TEST");
		std::wstring strMessage = stTestParam.client->Recv();
		if (strMessage.empty() || wcscmp(strMessage.c_str(), L"TEST"))	throw;

		return true;
	}
	catch (...)
	{
		printf("[ERROR CODE] %d\n", WSAGetLastError());
		return false;
	}
}

bool Connection_RecvTest(ST_TEST_PARAM stTestParam)
{
	try
	{
		stTestParam.client->Send(L"TEST");
		std::wstring strMessage = stTestParam.connection->Recv();
		if (strMessage.empty() || wcscmp(strMessage.c_str(), L"TEST"))	throw;

		return true;
	}
	catch (...)
	{
		printf("[ERROR CODE] %d\n", WSAGetLastError());
		return false;
	}
}

bool Connection_SendChatDataTest(ST_TEST_PARAM stTestParam)
{
	try
	{
		std::vector<std::wstring> vecChatData;
		vecChatData.push_back(L"TEST1");
		vecChatData.push_back(L"TEST2");
		vecChatData.push_back(L"TEST3");

		int nSendSize = 0;
		nSendSize = stTestParam.connection->SendChatData(vecChatData);

		std::vector<std::wstring> vecTmp = stTestParam.client->RecvChatData();

		if (nSendSize <= 0)	throw;

		return true;
	}
	catch (...)
	{
		printf("[ERROR CODE] %d\n", WSAGetLastError());
		return false;
	}
}

bool Connection_GetCurrentTimeStringTest(ST_TEST_PARAM stTestParam)
{
	try
	{
		std::wstring strTime;
		strTime = stTestParam.connection->GetCurrentTimeString();
		if (strTime.empty())	throw;

		return true;
	}
	catch (...)
	{
		printf("[ERROR CODE] %d\n", WSAGetLastError());
		return false;
	}
}

bool Client_RecvChatDataTest(ST_TEST_PARAM stTestParam)
{
	try
	{
		std::vector<std::wstring> vecSend;
		vecSend.push_back(L"TEST1");
		vecSend.push_back(L"TEST2");
		vecSend.push_back(L"TEST3");
		stTestParam.connection->SendChatData(vecSend);

		std::vector<std::wstring> vecRecv = stTestParam.client->RecvChatData();
		if (vecRecv.empty() || (vecRecv.size() != 3))	throw;

		return true;
	}
	catch (...)
	{
		printf("[ERROR CODE] %d\n", WSAGetLastError());
		return false;
	}
}

bool Client_setUserNameTest(ST_TEST_PARAM stTestParam)
{
	try
	{
		CChatClient client;
		client.setUserName(L"TEST");

		std::wstring strUserName = client.getUserName();
		if (wcscmp(strUserName.c_str(), L"TEST"))	throw;

		return true;
	}
	catch (...)
	{
		printf("[ERROR CODE] %d\n", WSAGetLastError());
		return false;
	}
}

bool Server_BroadcastTest(ST_TEST_PARAM stTestParam)
{
	try
	{

		return true;
	}
	catch (...)
	{
		printf("[ERROR CODE] %d\n", WSAGetLastError());
		return false;
	}
}

bool Server_UpdateChatDataTest(ST_TEST_PARAM stTestParam)
{
	try
	{

		return true;
	}
	catch (...)
	{
		printf("[ERROR CODE] %d\n", WSAGetLastError());
		return false;
	}
}
