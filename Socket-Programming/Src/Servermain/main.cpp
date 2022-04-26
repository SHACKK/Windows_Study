#include "pch.h"
#pragma comment(lib, "ws2_32.lib")

DWORD WINAPI ServerThreadCaller(void* pFuncName)
{
	CServer* server = (CServer*)(pFuncName);
	return server->ServerThread();
}

DWORD WINAPI ClientThreadCaller(void* pFuncName)
{
	CClient* client = (CClient*)pFuncName;
	return client->ClientThread();
}

int main(void)
{
	ST_WSA_INITIALIZER init;
	CServer server;
	CClient client;

	DWORD dwThreadId = 0;
	HANDLE hServerThread = ::CreateThread(nullptr, 0, ServerThreadCaller, &server, 0, &dwThreadId);

	Sleep(200);

	std::vector<HANDLE> vecClientThread;
	for (int i = 0; i < 5; i++)
	{
		HANDLE hClientThread = CreateThread(nullptr, 0, ClientThreadCaller, &client, 0, nullptr);
		vecClientThread.push_back(hClientThread);
		Sleep(500);
	}
	
	for (size_t i = 0; i < vecClientThread.size(); i++)
		WaitForSingleObject(vecClientThread[i], INFINITE);

	return 0;
}