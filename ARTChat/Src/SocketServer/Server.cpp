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

// Ŭ���̾�Ʈ�� ����
void CServer::AcceptThread()
{
	while (true)
	{
		if (m_queReady.empty())
			continue;

		SOCKET* hConnectionSocket = new SOCKET; //delete ��� �������..?
		sockaddr RemoteInfo;
		int nRemoteInfoSize = (int)sizeof(RemoteInfo);

		*hConnectionSocket = ::accept(m_ListenSocket, &RemoteInfo, &nRemoteInfoSize);

		CConnectionSuper* newConnection = m_queReady.front();
		newConnection->Establish(*hConnectionSocket, this);
		m_setConnected.insert(newConnection);
	}
}

// Ŭ���̾�Ʈ�� ���� ����
void CServer::DisAcceptThread()
{

}

// ����
// 1. AcceptThread(), DisAcceptThread(), ConnectionThread()�� CreateThread()�� ����� �ϴ°���.
// 2. ChatConnection���� StreamConnection���� ��� �����ϴ���? �б����� �ʿ��ϴٸ� ConnectionThread()���� �б��ϴ°� �´���.
// 3. 


// Ȯ���ؾ� �� ��.
// 1. Server�� Client���� ������ ���������ִ� ���ҿ��� ����Ѵ�. (ť ����, Accept)
// 2. Connection�� ���� ���Ŀ� � �۾��� �ϴ����� �����Ѵ�. (Chat�̳� Stream�� �ְ����)
// 3. 