#include "pch.h"
#pragma comment(lib, "Ws2_32.lib")

#define PORT 56000
#define MAX_CONNECTION 1


void CheckRet(int nRet)
{
	if (nRet == 1)
		printf("[SUCCESS] Server StartUp()\n");
	else
	{
		printf("[FAILED] Server StartUp()\n");
		printf("[ERROR CODE] %d\n", WSAGetLastError());
	}
}

int main()
{
	std::setlocale(LC_ALL, "ko_KR.UTF-8");
	int nRet;

	// 1.Server.StartUp()
	ST_WSA_INITIALIZER stWsaInit;
	CServer server;
	WORD nPort = 56000;
	std::vector<CConnectionSuper*> vecConnectionPool;
	for (int i = 0; i < MAX_CONNECTION; i++)
	{
		CChatConnection* connection = new CChatConnection;
		vecConnectionPool.push_back(connection);
	}
	ST_SERVER_INIT stServerInit(PORT, vecConnectionPool);
	nRet = server.StartUp(stServerInit);
	if (nRet == 1)
		printf("[SUCCESS] Server StartUp()\n");
	else
	{
		printf("[FAILED] Server StartUp()\n");
		printf("[ERROR CODE] %d\n", WSAGetLastError());
	}

	CChatClient client_1;
	CChatClient client_2;
	CChatClient client_3;

	// 2. Client.Connect()
	ST_SERVER_INFO stServerInfo;
	stServerInfo.IP = "127.0.0.1";
	stServerInfo.Port = 56000;
	nRet = client_1.Connect(stServerInfo);
	if (nRet == 1)
		printf("[SUCCESS] Client.Connect()\n");
	else
	{
		printf("[FAILED] Client.Connect()\n");
		printf("[ERROR CODE] %d\n", WSAGetLastError());
	}

	// 3. Server.Send() <-> Client.Recv()
	std::wstring strAccept = client_1.Recv();
	if (!wcscmp(strAccept.c_str(), L"Accept"))
	{
		printf("[SUCCESS] Server Send()\m");
		printf("[SUCCESS] Client Recv()\m");
	}
	else
	{
		printf("[FAILED] Server.Send() <-> Client.Recv()\n");
		printf("[ERROR CODE] %d \n", WSAGetLastError());
	}

	// 3. Client.Send() <-> Server.Recv()
	client_1.setUserName(L"TEST");
	nRet = client_1.Send(client_1.getUserName());
	if (nRet != 0)
		printf("[SUCCESS] Client Send()\n");
	else
	{
		printf("[FAILED] Client Send()\n");
		printf("%d\n", WSAGetLastError());
	}

	//4. Server.SendChatData() <-> Client.RecvChatData()

	return 0;
}