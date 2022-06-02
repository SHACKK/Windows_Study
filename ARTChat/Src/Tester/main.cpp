#include "pch.h"
#include "Connection.h"
#include "TestCase.h"
#pragma comment(lib, "Ws2_32.lib")

#define MAX_CONNECTION 5
#define MAX_CLIENT 5

void CheckResult(bool bRes, std::string strTestName)
{
	if (bRes)
		printf("[SUCCESS] %s\n", strTestName.c_str());
	else
		printf("[FAILED] %s\n", strTestName.c_str());
}

DWORD WINAPI ServerAccpetThreadCaller(LPVOID pContext)
{
	CServer& server = *(CServer*)pContext;
	return server.TestAcceptThread();
}

DWORD WINAPI ServerDisAcceptThreadCaller(LPVOID pContext)
{
	CServer& server = *(CServer*)pContext;
	return server.TestDisAcceptThread();
}

int main()
{
	std::setlocale(LC_ALL, "ko_KR.UTF-8");
	ST_WSA_INITIALIZER stWsaInit;

	CServer server;
	CChatConnection connection;
	CChatClient client;
	ST_TEST_PARAM stTestParam;
	stTestParam.server = &server;
	stTestParam.connection = &connection;
	stTestParam.client = &client;

	MakeConnection(stTestParam);

	CheckResult(Client_SendTest(stTestParam), "Client Send() Test");
	CheckResult(Client_RecvTest(stTestParam), "Client Recv() Test");
	CheckResult(Connection_SendTest(stTestParam), "Connection Send() Test");
	CheckResult(Connection_RecvTest(stTestParam), "Connection Recv() Test");
	CheckResult(Connection_SendChatDataTest(stTestParam), "Connection SendChatData() Test");
	CheckResult(Connection_GetCurrentTimeStringTest(stTestParam), "Connection GetCurrentTimeString() Test");
	CheckResult(Client_RecvChatDataTest(stTestParam), "Client RecvChatData() Test");
	CheckResult(Client_setUserNameTest(stTestParam), "Client SetUserName() Test");

	return 0;
}