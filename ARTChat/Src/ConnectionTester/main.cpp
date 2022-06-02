#include "pch.h"
#pragma comment(lib, "Ws2_32.lib")

#define PORT 56000
#define MAX_CONNECTION 20
#define MAX_WAITTING_CLIENT 10
std::vector<CChatClient*> vecClientPool;

struct ST_WSA_INITIALIZER_
{
	WSAData wsa;
	ST_WSA_INITIALIZER_(void)
	{
		WSAStartup(MAKEWORD(2, 2), &wsa);
	}
	~ST_WSA_INITIALIZER_(void)
	{
		WSACleanup();
	}
};

//1. 랜덤으로 vecConnectionPool에서 한명씩 나감
//2. 최대 50명의 사용자가 대기하고 있음
//3. 대기중인 사용자 중에서도 랜덤으로 나감
DWORD WINAPI ConnectToServer(LPVOID pContext)
{
	CChatClient* client = (CChatClient*)pContext;

	ST_SERVER_INFO stServerInfo;
	stServerInfo.IP = "127.0.0.1";
	stServerInfo.Port = PORT;

	client->Connect(stServerInfo, L"TEST_WAIT");
	client->m_vecChatData = client->RecvChatData();
	vecClientPool.push_back(client);
	
	return 0;
}

DWORD WINAPI UpdateWaittingVector(LPVOID pContext)
{
	std::vector<CChatClient*> vecWaittingClient;

	while (true)
	{
		if (vecWaittingClient.size() == MAX_WAITTING_CLIENT)
			continue;

		CChatClient* newWaittingClient = new CChatClient;
		::CreateThread(nullptr, 0, ConnectToServer, &newWaittingClient, 0, nullptr);
		vecWaittingClient.push_back(newWaittingClient);
	}
}

DWORD WINAPI RamdomExitFromConnected(LPVOID pContext)
{
	std::vector<CChatClient*>& vecClientPool = *(std::vector<CChatClient*>*)pContext;
	while (true)
	{
		int index = rand() % (MAX_CONNECTION - 1);
		vecClientPool.erase(vecClientPool.begin() + index);

		Sleep(1000);
	}
}

int main()
{
	ST_WSA_INITIALIZER_ stWsaInit;

	std::vector<CConnectionSuper*> vecConnectionPool;

	for (int i = 0; i < MAX_CONNECTION; i++)
	{
		CChatConnection* connection = new CChatConnection;
		vecConnectionPool.push_back(connection);
	}

	ST_SERVER_INIT stServerInit(PORT, vecConnectionPool);
	CServer server;
	server.StartUp(stServerInit);


	ST_SERVER_INFO stServerInfo;
	stServerInfo.IP = "127.0.0.1";
	stServerInfo.Port = PORT;

	
	for (int j = 0; j < MAX_CONNECTION; j++)
	{
		CChatClient* client = new CChatClient;
		client->Connect(stServerInfo, L"TEST" + std::to_wstring(j));
		client->m_vecChatData = client->RecvChatData();
		vecClientPool.push_back(client);
	}

	printf("First Connection Success\n");
	system("cls");

	HANDLE hUpdateWaittingVector = ::CreateThread(nullptr, 0, UpdateWaittingVector, nullptr, 0, nullptr);
	HANDLE hRamdomExitFromConnected = ::CreateThread(nullptr, 0, RamdomExitFromConnected, &vecClientPool, 0, nullptr);

	WaitForSingleObject(hUpdateWaittingVector, INFINITE);
	WaitForSingleObject(hRamdomExitFromConnected, INFINITE);

	return 0;
}

