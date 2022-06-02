#include "pch.h"
#pragma comment(lib, "Ws2_32.lib")

#define PORT 55000
#define MAX_CONNECTION 3

int main()
{
	ST_WSA_INITIALIZER stWsaInit;

	std::vector<CConnectionSuper*> vecConnectionPool;

	for (int i = 0; i < MAX_CONNECTION; i++)
	{
		CChatConnection* connection = new CChatConnection;
		vecConnectionPool.push_back(connection);
	}

	ST_SERVER_INIT stServerInit(PORT,vecConnectionPool);
	CServer server;
	server.StartUp(stServerInit);

	do
	{
		Sleep(1000);
	} while(true);

	server.ShutDown();
	
	return 0;
}