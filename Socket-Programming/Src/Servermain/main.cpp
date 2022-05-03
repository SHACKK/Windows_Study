#include "pch.h"
#pragma comment(lib, "ws2_32.lib")

std::map<std::wstring, std::wstring> v_UserIdData =
{
	{L"profrog", L"전상현"},
	{L"hyungsuki", L"김형석"},
	{L"", L"원종은"},
	{L"jfhg456", L"임창현"},
	{L"ploneska", L"김지수"},
	{L"aoi", L"이준성"}
};

std::vector<std::wstring> v_ChatData = { {L"채팅시작.."} };
CMessage msg;
std::mutex mtx;

DWORD WINAPI ConnectionThread(LPVOID stThreadArg)
{
	ST_CONNECTION_PARAM& Param = *(ST_CONNECTION_PARAM*)stThreadArg;
	CSocketConnection connect = (CSocketConnection)(Param.connect);
	connect.Create();

	//최초 : UserId 확인, 채팅 데이터 전송
	std::wstring strUserId = v_UserIdData[connect.Recv()];
	v_ChatData = Param.msg->GetMessgae();
	connect.Send(std::to_wstring(v_ChatData.size()));
	for (size_t i = 0; i < v_ChatData.size(); i++)
	{
		connect.Send(v_ChatData[i]);
	}

	while (true)
	{
		std::wstring strRecvMsg = connect.Recv();
		mtx.lock();
		Param.msg->InsertMessage(strUserId, strRecvMsg.c_str());
		v_ChatData = Param.msg->GetMessgae();
		Param.server->BroadCast(v_ChatData);
		mtx.unlock();
	}
}

int main()
{
	std::setlocale(LC_ALL, "ko_KR.UTF-8");

	ST_WSA_INITIALIZER init;
	printf("Server Startup..\n");

	CSocketServer server;
	ST_CONNECTION_PARAM stThreadArg;
	stThreadArg.server = &server;
	stThreadArg.msg = &msg;

	while (true)
	{
		CSocketConnection conn;
		conn.hConnectionSocket = server.Listen();

		if (INVALID_SOCKET == conn.hConnectionSocket)
			continue;
		server.ListSocket.push_back(conn.hConnectionSocket);
		stThreadArg.connect = conn;
		CreateThread(nullptr, 0, ConnectionThread, &stThreadArg, 0, nullptr);
	}

	return 0;
}