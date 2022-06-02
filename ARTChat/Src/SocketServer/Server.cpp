#include "pch.h"
#include "Server.h"
#include "ConnectionSuper.h"
#include "ChatConnection.h"

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
	Broadcast(CONNECTION_CLOSE_BY_SERVER);
	WaitForSingleObject(hAcceptThread, INFINITE);
	WaitForSingleObject(hDisAcceptThread, INFINITE);
}

void CServer::Broadcast(std::wstring strMessage)
{
	size_t tMessageLength = strMessage.length() * sizeof(wchar_t);

	std::set<CConnectionSuper*>::iterator iter;
	for (iter = m_setConnected.begin(); iter != m_setConnected.end(); iter++)
	{
		(*iter)->Send((LPCBYTE)&tMessageLength, sizeof(tMessageLength));
		(*iter)->Send((LPCBYTE)(strMessage.c_str()), tMessageLength);
	}
}

void CServer::InsertConnectedSet(CConnectionSuper* newConnection)
{
	mtx_setConnected.lock();
	m_setConnected.insert(newConnection);
	mtx_setConnected.unlock();

	wprintf(L"[INFO] Client connected!! (%d/%d)\n", (int)m_setConnected.size(), MAX_CONNECTION_SIZE);
}

std::vector<std::wstring> CServer::GetChatData()
{
	return m_vecChatData;
}

bool CServer::UpdateChatData(std::wstring strMessage)
{
	mtx_ChatData.lock();
	if (MAX_CHATDATA_SIZE < m_vecChatData.size())	
		m_vecChatData.erase(m_vecChatData.begin());
	m_vecChatData.push_back(strMessage);

	Broadcast(strMessage);

	// Broadcast 과정에서 순서가 뒤바뀔 수도 있으므로 이 행위를 포함하여 mutex처리를 해줬다.
	mtx_ChatData.unlock();	

	return true;
}

SOCKET CServer::GetListenSocket()
{
	return m_ListenSocket;
}

void CServer::DisConnect(CConnectionSuper* pConnection)
{
	mtx_setConnected.lock();
	m_setConnected.erase(pConnection);
	mtx_setConnected.unlock();

	wprintf(L"[INFO] Client disconnected!! (%d/%d)\n", (int)m_setConnected.size(), MAX_CONNECTION_SIZE);

	mtx_queDiscon.lock();
	m_queDiscon.push(pConnection);
	mtx_queDiscon.unlock();

	
}

// 클라이언트와 연결
DWORD CServer::AcceptThread()
{
	while (true)
	{
		sockaddr RemoteInfo;
		int nRemoteInfoSize = (int)sizeof(RemoteInfo);

		SOCKET hConnectionSocket = ::accept(m_ListenSocket, &RemoteInfo, &nRemoteInfoSize);

		if (m_queReady.empty())
		{
			mtx_queSuspended.lock();
			m_queSuspended.push(hConnectionSocket);
			mtx_queSuspended.unlock();

			std::wstring strWaitMessage = L"Wait";
			size_t nLength = strWaitMessage.length() * sizeof(wchar_t);
			::send(hConnectionSocket, (const char*)&nLength, (int)sizeof(nLength), 0);
			::send(hConnectionSocket, (const char*)strWaitMessage.c_str(), (int)nLength, 0);

			continue;
		}

		mtx_queReady.lock();
		CConnectionSuper* newConnection = m_queReady.front();
		m_queReady.pop();
		mtx_queReady.unlock();

		newConnection->Establish(hConnectionSocket, this);
	}
}

// 클라이언트와 연결 해제
DWORD CServer::DisAcceptThread()
{
	while (true)
	{
		if (m_queDiscon.empty())
			continue;

		if (m_queSuspended.empty()) // 끊어진 연결이 있고, 대기중인 소켓이 없는 경우
		{
			mtx_queDiscon.lock();
			CConnectionSuper* closedConnection = m_queDiscon.front();
			m_queDiscon.pop();
			mtx_queDiscon.unlock();

			mtx_queReady.lock();
			m_queReady.push(closedConnection);
			mtx_queReady.unlock();
		}
		else // 끊어진 연결이 있고, 대기중인 소켓이 있는 경우
		{
			mtx_queSuspended.lock();
			SOCKET suspendedSocket = m_queSuspended.front();
			m_queSuspended.pop();
			mtx_queSuspended.unlock();

			CConnectionSuper* closedConnection = m_queDiscon.front();
			m_queDiscon.pop();
			closedConnection->Establish(suspendedSocket, this);
		}
	}
}

void CServer::MakeConnectionPool(std::vector<CConnectionSuper*> vecConnectionPool)
{
	// Fill the m_queReady
	for (auto iter : vecConnectionPool)
		m_queReady.push(iter);
	if (m_queReady.empty()) throw std::exception("Connection Pool doesn't exist");
}

DWORD CServer::TestAcceptThread()
{
	while (true)
	{
		sockaddr RemoteInfo;
		int nRemoteInfoSize = (int)sizeof(RemoteInfo);

		SOCKET hConnectionSocket = ::accept(m_ListenSocket, &RemoteInfo, &nRemoteInfoSize);

		if (m_queReady.empty())
		{
			mtx_queSuspended.lock();
			m_queSuspended.push(hConnectionSocket);
			mtx_queSuspended.unlock();

			std::wstring strWaitMessage = L"Wait";
			size_t nLength = strWaitMessage.length() * sizeof(wchar_t);
			::send(hConnectionSocket, (const char*)&nLength, (int)sizeof(nLength), 0);
			::send(hConnectionSocket, (const char*)strWaitMessage.c_str(), (int)nLength, 0);

			continue;
		}

		mtx_queReady.lock();
		CConnectionSuper* newConnection = m_queReady.front();
		m_queReady.pop();
		mtx_queReady.unlock();

		//Accept 보내기
		std::wstring strAccept = L"Accept";
		size_t nLength_ = strAccept.length() * sizeof(wchar_t);
		newConnection->Send((LPCBYTE)&nLength_, sizeof(nLength_));
		newConnection->Send((LPCBYTE)strAccept.c_str(), nLength_);

		//strUserName 받기
		size_t nLength = 0;
		newConnection->Recv((LPBYTE)&nLength, sizeof(nLength));

		std::wstring strUserName;
		strUserName.resize(nLength / sizeof(wchar_t));
		newConnection->Recv((LPBYTE)strUserName.c_str(), (int)nLength);

		InsertConnectedSet(newConnection);
	}
}

DWORD CServer::TestDisAcceptThread()
{
	while (true)
	{
		if (m_queDiscon.empty())
			continue;

		if (m_queSuspended.empty()) // 끊어진 연결이 있고, 대기중인 소켓이 없는 경우
		{
			mtx_queDiscon.lock();
			CConnectionSuper* closedConnection = m_queDiscon.front();
			m_queDiscon.pop();
			mtx_queDiscon.unlock();

			mtx_queReady.lock();
			m_queReady.push(closedConnection);
			mtx_queReady.unlock();
		}
		else // 끊어진 연결이 있고, 대기중인 소켓이 있는 경우
		{
			mtx_queSuspended.lock();
			SOCKET suspendedSocket = m_queSuspended.front();
			m_queSuspended.pop();
			mtx_queSuspended.unlock();

			CConnectionSuper* closedConnection = m_queDiscon.front();
			m_queDiscon.pop();
			InsertConnectedSet(closedConnection);
		}
	}
}