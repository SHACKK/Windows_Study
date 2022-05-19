#include "pch.h"
#include "Server.h"
#include "ConnectionSuper.h"

int CServer::StartUp(ST_SERVER_INIT stInit)
{
	try {
		m_ListenSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == m_ListenSocket) throw WSAGetLastError();

		sockaddr_in service;
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = INADDR_ANY;
		service.sin_port = htons(stInit.wPort);
		int nExcep = ::bind(m_ListenSocket, (sockaddr*)&service, sizeof(service));
		if (SOCKET_ERROR == nExcep) throw std::exception("Failed to Bind");

		nExcep = listen(m_ListenSocket, 200);
		if (SOCKET_ERROR == nExcep) throw std::exception("Failed to Listen");

		// Fill the m_queReady
		m_vecConnectionInstance = stInit.vecConnections;
		for (auto iter : m_vecConnectionInstance)
			m_queReady.push(iter);
		if (m_queReady.empty()) throw std::exception("Connection Pool doesn't exist");

		printf("[SUCCESS] Server StartUp...\n");

		HANDLE hAcceptThread = ::CreateThread(nullptr, 0, AcceptThread, this, 0, nullptr);
		HANDLE hDisAcceptThread = ::CreateThread(nullptr, 0, DisAcceptThread, this, 0, nullptr);

		return 0;
	}
	catch (std::exception& e)
	{
		printf("[Error] : %s\n %d\n", e.what(), WSAGetLastError());
		return 1;
	}
}

void CServer::ShutDown()
{
}

void CServer::Broadcast(LPBYTE pContext, size_t Size)
{
}

std::vector<std::string> CServer::GetChatData()
{
	return m_vecChatData;
}

bool CServer::PushChatMessage(std::string strMessage)
{
	mtx.lock();
	m_vecChatData.push_back(strMessage);
	mtx.unlock();

	return true;
}

// 클라이언트와 연결
DWORD WINAPI CServer::AcceptThread(LPVOID pContext)
{
	CServer& server = *(CServer*)pContext;
	while (true)
	{
		if (server.m_queReady.empty())
			continue;

		sockaddr RemoteInfo;
		int nRemoteInfoSize = (int)sizeof(RemoteInfo);

		SOCKET hConnectionSocket = ::accept(server.m_ListenSocket, &RemoteInfo, &nRemoteInfoSize);

		CConnectionSuper* newConnection = server.m_queReady.front();
		newConnection->Establish(hConnectionSocket, &server);
		server.m_setConnected.insert(newConnection);
	}
}

// 클라이언트와 연결 해제
DWORD WINAPI CServer::DisAcceptThread(LPVOID pContext)
{
	CServer& server = *(CServer*)pContext;
	while (true)
	{
		if (server.m_queDiscon.empty())
			continue;

		CConnectionSuper* closedConnection = server.m_queDiscon.front();
		server.m_queDiscon.pop();
		server.m_queReady.push(closedConnection);
	}
}