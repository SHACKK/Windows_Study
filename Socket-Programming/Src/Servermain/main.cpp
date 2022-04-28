#include "pch.h"
#pragma comment(lib, "ws2_32.lib")

std::map<std::wstring, std::wstring> v_UserIdData =
{
	{L"jfhg456", L"임창현"}
};

std::vector<std::wstring> v_ChatData = { {L"<<채팅 시작>>"} };

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
	std::wstring strRecvMsg = strUserId + L" : ";

	std::wstring strVecSize = std::to_wstring(v_ChatData.size());
	connect->Send(strVecSize);

	for (size_t i = 0; i < v_ChatData.size(); i++)
	{
		connect->Send(v_ChatData[i]);
	}

	while (true)
	{
		strRecvMsg += connect->Recv();
		v_ChatData.push_back(strRecvMsg.c_str());

		//브로드캐스팅 해야함
		for (size_t i = 0; i < v_ChatData.size(); i++)
		{
			connect->Send(v_ChatData[i]);
		}

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