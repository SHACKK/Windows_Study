#include "pch.h"
#pragma comment(lib, "ws2_32.lib")

std::map<std::wstring, std::wstring> v_UserIdData =
{
	{L"jfhg456", L"임창현"}
};
std::vector<std::wstring> v_ChatData = { {L"채팅시작.."} };

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

CSocketServer server;

DWORD ConnectionCreateCaller(void* pInstance)
{
	CSocketConnection* connect = (CSocketConnection*)(pInstance);
	connect->Create();

	//서버 부하를 줄이기 위해서는 Client에서 해당작업을 하는게 좋아보임
	std::wstring strUserId = v_UserIdData[connect->Recv()];
	std::wstring strRecvMsg = strUserId + L" : ";

	//v_ChatData의 크기 전송
	connect->Send(std::to_wstring(v_ChatData.size()));
	//v_ChatData 데이터 전송
	for (size_t i = 0; i < v_ChatData.size(); i++)
	{
		connect->Send(v_ChatData[i]);
	}

	while (true)
	{
		strRecvMsg += connect->Recv();
		v_ChatData.push_back(strRecvMsg.c_str());

		//브로드캐스팅 해야함
		//c++ 문법 문제
		server.BroadCast(v_ChatData);
	}
}

int main()
{
	std::setlocale(LC_ALL, "ko_KR.UTF-8");

	ST_WSA_INITIALIZER init;
	printf("Server Startup..\n");

	while (true)
	{
		CSocketConnection conn;
		conn.hConnectionSocket = server.Listen();

		if (INVALID_SOCKET == conn.hConnectionSocket)
			continue;
		server.ListSocket.push_back(conn.hConnectionSocket);

		CreateThread(nullptr, 0, ConnectionCreateCaller, &conn, 0, nullptr);
	}

	return 0;
}