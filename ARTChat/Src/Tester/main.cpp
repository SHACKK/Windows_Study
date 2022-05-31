#include "pch.h"
#pragma comment(lib, "Ws2_32.lib")

#define PORT 56000
#define MAX_CONNECTION 1

struct ST_ACCEPTTHREAD_PARAM
{
	CServer* server;
	CChatConnection* connection;
};

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
	catch (std::exception& e)
	{
		printf("%s", e.what());
	}
	
}

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

void CheckResult(bool bRes, std::string strTestName)
{
	if (bRes)
		printf("[SUCCESS] %s", strTestName.c_str());
	else
		printf("[FAILED] %s", strTestName.c_str());
}

int main()
{
	std::setlocale(LC_ALL, "ko_KR.UTF-8");
	ST_WSA_INITIALIZER stWsaInit;
	bool bRes;

	bRes = SendTest();
	CheckResult(bRes, "SendTest");

	return 0;
}