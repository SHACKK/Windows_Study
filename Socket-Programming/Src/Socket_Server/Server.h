#pragma once
#include <vector>

class CSocketServer
{
public:
	CSocketServer(){};
	~CSocketServer() {};

	SOCKET hListenSocket;
	int nRet;
	std::list<SOCKET> ListSocket;

	SOCKET Listen();
	void BroadCast(std::vector<std::wstring> v_ChatData);
	void Close();
};