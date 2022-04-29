#include "pch.h"
#pragma comment(lib, "ws2_32.lib")

std::map<std::wstring, std::wstring> v_UserIdData =
{
	{L"jfhg456", L"��â��"}
};
std::vector<std::wstring> v_ChatData = { {L"ä�ý���.."} };

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

	//���� ���ϸ� ���̱� ���ؼ��� Client���� �ش��۾��� �ϴ°� ���ƺ���
	std::wstring strUserId = v_UserIdData[connect->Recv()];
	std::wstring strRecvMsg = strUserId + L" : ";

	//v_ChatData�� ũ�� ����
	connect->Send(std::to_wstring(v_ChatData.size()));
	//v_ChatData ������ ����
	for (size_t i = 0; i < v_ChatData.size(); i++)
	{
		connect->Send(v_ChatData[i]);
	}

	while (true)
	{
		strRecvMsg += connect->Recv();
		v_ChatData.push_back(strRecvMsg.c_str());

		//��ε�ĳ���� �ؾ���
		//c++ ���� ����
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