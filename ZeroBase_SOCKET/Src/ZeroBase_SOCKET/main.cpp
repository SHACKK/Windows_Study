#include <Windows.h>
#include <stdio.h>
#include "Packetizer.h"
#pragma comment(lib, "Ws2_32.lib")

struct ST_SERVERTHREAD_PARAM
{
	SOCKET* hListenSocket;
	SOCKET* hConnectionSocket;
};

DWORD WINAPI ServerThread(LPVOID pContext)
{
	ST_SERVERTHREAD_PARAM& stParam = *(ST_SERVERTHREAD_PARAM*)pContext;
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_port = htons(56000);
	service.sin_addr.s_addr = INADDR_ANY;
	::bind(*(stParam.hListenSocket), (sockaddr*)&service, sizeof(service));
	::listen(*(stParam.hListenSocket), 200);

	sockaddr RemoteInfo;
	int nRemoteInfoSize = (int)sizeof(RemoteInfo);
	*(stParam.hConnectionSocket) = ::accept(*(stParam.hListenSocket), &RemoteInfo, &nRemoteInfoSize);
	if (*(stParam.hConnectionSocket) == INVALID_SOCKET)
	{
		return 0;
	}

	PACKET_HEADER packet;
	::recv(*(stParam.hConnectionSocket), (char*)&packet, sizeof(PACKET_HEADER), MSG_PEEK);
	CPacketizer packetizer;
	packetizer.ProcessPacket(&packet, sizeof(PACKET_HEADER));

	return 0;
}

DWORD WINAPI ClientThread(LPVOID pContext)
{
	SOCKET& hClientSocket = *(SOCKET*)pContext;
	sockaddr_in service_;
	service_.sin_family = AF_INET;
	service_.sin_addr.s_addr = inet_addr("127.0.0.1");
	service_.sin_port = htons(56000);
	int nRet = ::connect(hClientSocket, (sockaddr*)&service_, sizeof(service_));
	if (nRet == SOCKET_ERROR)
	{
		return 0;
	}

	std::string strUserId = "jfhg456";
	REQ_CONNECT pckConnect;
	strcpy_s(pckConnect.szUserID, strUserId.c_str());
	send(hClientSocket, (const char*)&pckConnect, (int)sizeof(REQ_CONNECT), 0);

	return 0;
}

int main()
{
	WSAData wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET hListenSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKET hClientSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKET hConnectionSocket;
	ST_SERVERTHREAD_PARAM stParam;
	stParam.hListenSocket = &hListenSocket;
	stParam.hConnectionSocket = &hConnectionSocket;
	HANDLE hServerHandle = ::CreateThread(nullptr, 0, ServerThread, &stParam, 0, nullptr);
	HANDLE hClientHandle = ::CreateThread(nullptr, 0, ClientThread, &hClientSocket, 0, nullptr);

	WaitForSingleObject(hServerHandle, INFINITE);
	WaitForSingleObject(hClientHandle, INFINITE);

	WSACleanup();
	return 0;
}