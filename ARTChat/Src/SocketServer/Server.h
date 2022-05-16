#pragma once

class CConnectionSuper;

struct ST_SERVER_INIT
{
	WORD wPort;
	std::vector<CConnectionSuper*> connections;
};

class CServer
{
private:
	std::vector<CConnectionSuper*> m_vecConnectionInstance;
	std::queue<CConnectionSuper*> m_queReady;
	std::set<CConnectionSuper*> m_setConnected;
	std::queue<CConnectionSuper*> m_queDiscon;
	SOCKET m_ListenSocket;
public:
	int StartUp(ST_SERVER_INIT stInit);
	void ShutDown();
	void Broadcast();
	void AcceptThread();
	void DisconnectThread();
};
