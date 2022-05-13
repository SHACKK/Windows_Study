#pragma once
#include "Struct.h"

class CConnectionSuper;

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
