#include "pch.h"
#include <vector>
#include <Windows.h>

CServer server;
CClient client;
CConnection connect;

int main(void)
{
	
	DWORD dwThreadId = 0;
	HANDLE hServerThread = ::CreateThread(nullptr, 0, server.ServerThread, nullptr, 0, &dwThreadId);

	Sleep(200);

	std::vector<HANDLE> vecClientThread;
	for (int i = 0; i < 5; i++)
	{
		HANDLE hClientThread = CreateThread(nullptr, 0, ClientThread, nullptr, 0, nullptr);
		vecClientThread.push_back(hClientThread);
		Sleep(500);
	}
	
	for (size_t i = 0; i < vecClientThread.size(); i++)
		WaitForSingleObject(vecClientThread[i], INFINITE);

	return 0;
}