#pragma once

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
	std::set<CConnectionSuper*> m_setConnected;
	std::queue<CConnectionSuper*> m_queDiscon;
	SOCKET m_ListenSocket;
	HANDLE hAcceptThread;
	HANDLE hDisAcceptThread;

	std::vector<std::string> m_vecChatData;
	std::mutex mtx;
public:
	DWORD AcceptThread();
	DWORD DisAcceptThread();
	int StartUp(ST_SERVER_INIT stInit);
	void ShutDown();
	void Broadcast(LPBYTE pConetxt, size_t Size);

	std::vector<std::string> GetChatData();
	bool PushChatMessage(std::string strMessage);
	void DisConnect(CConnectionSuper* pConnection);
};
