#include "pch.h"
#include "Server.h"
#include "ConnectionSuper.h"

DWORD WINAPI AcceptThreadCaller(void* pContext)
{
	CServer* server = (CServer*)pContext;
	return server->AcceptThread();
}

DWORD WINAPI DisAcceptThreadCaller(void* pContext)
{
	CServer* server = (CServer*)pContext;
	return server->DisAcceptThread();
}

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

		hAcceptThread = ::CreateThread(nullptr, 0, AcceptThreadCaller, this, 0, nullptr);
		hDisAcceptThread = ::CreateThread(nullptr, 0, DisAcceptThreadCaller, this, 0, nullptr);

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
	WaitForSingleObject(hAcceptThread, INFINITE);
	WaitForSingleObject(hDisAcceptThread, INFINITE);
}

void CServer::Broadcast(std::wstring strMessage)
{
	std::set<CConnectionSuper*>::iterator iter;
	for (iter = m_setConnected.begin(); iter != m_setConnected.end(); iter++)
	{
		(*iter)->Send(strMessage);
	}
}

void CServer::BroadcastChatData()
{
	mtx.lock();
	std::vector<std::wstring> vecTmp = GetChatData();
	mtx.unlock();

	std::set<CConnectionSuper*>::iterator iter;
	for (iter = m_setConnected.begin(); iter != m_setConnected.end(); iter++)
	{
		(*iter)->SendChatData(vecTmp);
	}
}

void CServer::InsertConnectedSet(CConnectionSuper* newConnection)
{
	m_setConnected.insert(newConnection);
}

std::vector<std::wstring> CServer::GetChatData()
{
	return m_vecChatData;
}

bool CServer::UpdateChatData(std::wstring strMessage)
{
	mtx.lock();
	m_vecChatData.push_back(strMessage);
	Broadcast(strMessage);
	mtx.unlock();

	return true;
}

void CServer::DisConnect(CConnectionSuper* pConnection)
{
	m_setConnected.erase(pConnection);
	m_queDiscon.push(pConnection);
}

// 클라이언트와 연결
DWORD CServer::AcceptThread()
{
	while (true)
	{
		if (m_queReady.empty())
			continue;

		sockaddr RemoteInfo;
		int nRemoteInfoSize = (int)sizeof(RemoteInfo);

		SOCKET hConnectionSocket = ::accept(m_ListenSocket, &RemoteInfo, &nRemoteInfoSize);

		CConnectionSuper* newConnection = m_queReady.front();
		m_queReady.pop();
		newConnection->Establish(hConnectionSocket, this);
		//m_setConnected.insert(newConnection); -> 채팅 보낸 이후에 추가하는 것으로 수정
	}
}

// 클라이언트와 연결 해제
DWORD CServer::DisAcceptThread()
{
	while (true)
	{
		if (m_queDiscon.empty())
			continue;

		CConnectionSuper* closedConnection = m_queDiscon.front();
		m_queDiscon.pop();
		m_queReady.push(closedConnection);
	}
}