#include "pch.h"
#pragma comment(lib, "ws2_32.lib")

std::map<std::wstring, std::wstring> v_UserIdData =
{
	{L"jfhg456", L"ÀÓÃ¢Çö"}
};

std::map<std::wstring, std::wstring> v_ChatData;
std::vector<std::vector<std::wstring, std::wstring>> v_DashBoard;

struct ST_WSA_INITIALIZER
{
	WSAData wsa;
	ST_WSA_INITIALIZER(void)
	{
		WSAStartup(MAKEWORD(2, 2), &wsa);
	}
	~ST_WSA_INITIALIZER(void)
	{
		WSACleanup();
	}
};

DWORD ConnectionCreateCaller(void* pInstance)
{
	CSocketConnection* connect = (CSocketConnection*)(pInstance);
	connect->Create();

	std::wstring strUserId = v_UserIdData[connect->Recv()];

	while (true)
	{
		std::wstring RecvMsg = connect->Recv();
		v_ChatData.insert(std::pair<std::wstring, std::wstring>(strUserId, RecvMsg));
		v_DashBoard.push_back();

	}
}

int main()
{
	ST_WSA_INITIALIZER init;
	CSocketServer server;
	printf("Server Startup");
	
	while (true)
	{
		CSocketConnection conn;
		conn.hConnectionSocket = server.Listen();

		CreateThread(nullptr, 0, ConnectionCreateCaller, &conn, 0, nullptr);
	}

	return 0;
}