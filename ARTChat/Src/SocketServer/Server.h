#pragma once

#define MAX_CHATDATA_SIZE 30

class CConnectionSuper;

struct ST_SERVER_INIT
{
	WORD wPort;
	std::vector<CConnectionSuper*> vecConnections;

	ST_SERVER_INIT(WORD PORT_NUM, std::vector<CConnectionSuper*> Connections)
		: wPort(PORT_NUM)
		, vecConnections(Connections)
	{}
};

class CServer
{
private:
	std::vector<CConnectionSuper*> m_vecConnectionInstance;
	std::queue<CConnectionSuper*> m_queReady;
	std::queue<SOCKET> m_queSuspended;
	std::set<CConnectionSuper*> m_setConnected;
	std::queue<CConnectionSuper*> m_queDiscon;
	SOCKET m_ListenSocket;
	HANDLE hAcceptThread;
	HANDLE hDisAcceptThread;

	std::vector<std::wstring> m_vecChatData = 
	{
		L"-------------------------Start Chat-------------------------"
	};

	std::mutex mtx_queReady;
	std::mutex mtx_queSuspended;
	std::mutex mtx_setConnected;
	std::mutex mtx_queDiscon;
	std::mutex mtx_ChatData;

public:
	CServer() {};
	~CServer()
	{
		std::wstring strShutdownCommand = L"/CloseByServer";
		Broadcast(strShutdownCommand);

	};
	DWORD AcceptThread();
	DWORD DisAcceptThread();
	int StartUp(ST_SERVER_INIT stInit);
	void ShutDown();
	void DisConnect(CConnectionSuper* pConnection);
	void Broadcast(std::wstring strMessage);
	void BroadcastChatData();
	void InsertConnectedSet(CConnectionSuper* newConnection);
	std::vector<std::wstring> GetChatData();
	bool UpdateChatData(std::wstring strMessage);
};
