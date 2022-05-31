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
	if (nRet == 0)
		printf("[SUCCESS] Server StartUp()\n");
	else
	{
		printf("[FAILED] Server StartUp()\n");
		printf("[ERROR CODE] %d\n", WSAGetLastError());
	}

	return 0;
}