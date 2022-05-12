#include "pch.h"
#pragma comment(lib, "ws2_32.lib")

std::map<std::wstring, std::wstring> v_UserIdData =
{
	{L"profrog", L"������"},
	{L"hyungsuki", L"������"},
	{L"Gracebell", L"������"},
	{L"jfhg456", L"��â��"},
	{L"ploneska", L"������"},
	{L"aoi", L"���ؼ�"}
};

std::mutex mtx;

DWORD WINAPI BroadCastThread(LPVOID stThreadArg)
{
	ST_CONNECTION_PARAM& Param = *(ST_CONNECTION_PARAM*)stThreadArg;
	CSocketServer& server = *(CSocketServer*)Param.server;
	CMessage& msg = *(CMessage*)Param.msg;

	mtx.lock();
	//(1)
	std::list<CSocketConnection*>::iterator iter;
	for (iter = server.ListSocket.begin();
			iter != server.ListSocket.end();
			iter++)
	{
		(*iter)->Send(msg.GetMessage());
	}

	//(2)
	for (auto it : server.ListSocket)
		(it)->Send(msg.GetMessage());

	mtx.unlock();

	return 0;
}

DWORD WINAPI RecvMsgThread(LPVOID stThreadArg)
{
	ST_CONNECTION_PARAM& Param = *(ST_CONNECTION_PARAM*)stThreadArg;
	CSocketConnection connect = (CSocketConnection)(Param.connect);

	// ���� ���� �� : UserId Ȯ��, ä�� ������ ����
	std::wstring strUserId = v_UserIdData[connect.Recv()];
	connect.Create(strUserId);
	connect.Send(Param.msg->GetMessage());

	while (true)
	{
		// v_ChatData�� ��� ������ �ִ� ��..?
		std::wstring strRecvMsg = connect.Recv();

		mtx.lock();
		Param.msg->InsertMessage(strUserId, strRecvMsg.c_str());
		mtx.unlock();

		::CreateThread(nullptr, 0, BroadCastThread, &Param, 0, nullptr);
	}
}

DWORD WINAPI ConnectionThread(LPVOID pContext)
{
	CSocketServer& server = *(CSocketServer*)pContext;
	CSocketConnection connect;
	connect.hConnectionSocket = server.Listen();

	if (INVALID_SOCKET == connect.hConnectionSocket)
		return 0;

	server.ListSocket.push_back(&connect);
	stThreadArg.connect = connect;
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
		server.ListSocket.push_back(&conn);
		stThreadArg.connect = conn;
		CreateThread(nullptr, 0, RecvMsgThread, &stThreadArg, 0, nullptr);
	}

	return 0;
}