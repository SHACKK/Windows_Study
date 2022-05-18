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
		if (SOCKET_ERROR == nExcep) throw std::exception("Failed to Bind..");

		nExcep = listen(m_ListenSocket, 200);
		if (SOCKET_ERROR == nExcep) throw std::exception("Failed to Listen...");

		// Fill the m_queReady
		m_vecConnectionInstance = stInit.vecConnections;
		for (auto iter : m_vecConnectionInstance)
			m_queReady.push(iter);
		if (m_queReady.empty()) throw std::exception("Connection Pool doesn't exist..");

		printf("Server StartUp...\n");

		while (true)
		{
			//::CreateThread(nullptr, 0, AcceptThread, nullptr, 0, nullptr);
			AcceptThread();

			while (!m_queDiscon.empty())
			{
				CConnectionSuper* connections = m_queDiscon.front();
				m_queDiscon.pop();
				m_queReady.push(connections);
			}
		}

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

// 클라이언트와 연결
void CServer::AcceptThread()
{
	while (true)
	{
		if (m_queReady.empty())
			continue;

		SOCKET* hConnectionSocket = new SOCKET; //delete 어디서 해줘야함..?
		sockaddr RemoteInfo;
		int nRemoteInfoSize = (int)sizeof(RemoteInfo);

		*hConnectionSocket = ::accept(m_ListenSocket, &RemoteInfo, &nRemoteInfoSize);

		CConnectionSuper* newConnection = m_queReady.front();
		newConnection->Establish(*hConnectionSocket, this);
		m_setConnected.insert(newConnection);
	}
}

// 클라이언트와 연결 해제
void CServer::DisAcceptThread()
{

}

// 질문
// 1. AcceptThread(), DisAcceptThread(), ConnectionThread()를 CreateThread()로 열어야 하는건지.
// 2. ChatConnection인지 StreamConnection인지 어디서 구분하는지? 분기점이 필요하다면 ConnectionThread()에서 분기하는게 맞는지.
// 3. 


// 확실해야 할 것.
// 1. Server는 Client와의 연결을 성립시켜주는 역할에만 충실한다. (큐 관리, Accept)
// 2. Connection은 연결 이후에 어떤 작업을 하는지를 정의한다. (Chat이나 Stream을 주고받음)
// 3. 