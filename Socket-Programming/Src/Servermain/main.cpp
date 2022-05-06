#include "pch.h"
#pragma comment(lib, "ws2_32.lib")

std::map<std::wstring, std::wstring> v_UserIdData =
{
	{L"profrog", L"전상현"},
	{L"hyungsuki", L"김형석"},
	{L"Gracebell", L"원종은"},
	{L"jfhg456", L"임창현"},
	{L"ploneska", L"김지수"},
	{L"aoi", L"이준성"}
};

std::mutex mtx;

DWORD WINAPI BroadCastThread(LPVOID stThreadArg)
{
	ST_CONNECTION_PARAM& Param = *(ST_CONNECTION_PARAM*)stThreadArg;

	mtx.lock();
	std::list<CSocketConnection>::iterator iter;
	for (iter = Param.server->ListSocket.begin();
			iter != Param.server->ListSocket.end();
			iter++)
	{
		iter->Send(Param.msg->GetMessgae());
	}
	mtx.unlock();

	return 0;
}

DWORD WINAPI RecvMsgThread(LPVOID stThreadArg)
{
	ST_CONNECTION_PARAM& Param = *(ST_CONNECTION_PARAM*)stThreadArg;

	CSocketConnection connect = (CSocketConnection)(Param.connect);

	// 최초 연결 시 : UserId 확인, 채팅 데이터 전송
	std::wstring strUserId = v_UserIdData[connect.Recv()];
	connect.Create(strUserId);
	connect.Send(Param.msg->GetMessgae());

	while (true)
	{
		// v_ChatData를 계속 보내고 있는 듯..?
		std::wstring strRecvMsg = connect.Recv();

		mtx.lock();
		Param.msg->InsertMessage(strUserId, strRecvMsg.c_str());
		mtx.unlock();

		::CreateThread(nullptr, 0, BroadCastThread, &stThreadArg, 0, nullptr);
	}
}

int main()
{
	std::setlocale(LC_ALL, "ko_KR.UTF-8");

	ST_WSA_INITIALIZER init;
	CSocketServer server;
	CMessage msg;

	ST_CONNECTION_PARAM stThreadArg;
	stThreadArg.server = &server;
	stThreadArg.msg = &msg;

	printf("Server Startup..\n");

	while (true)
	{
		CSocketConnection conn;
		conn.hConnectionSocket = server.Listen();

		if (INVALID_SOCKET == conn.hConnectionSocket)
			continue;
		server.ListSocket.push_back(conn);
		stThreadArg.connect = conn;
		CreateThread(nullptr, 0, RecvMsgThread, &stThreadArg, 0, nullptr);
	}

	return 0;
}